function printVertexArray ms obj f nmatId = 
(
	local trisCount = getNumFaces ms
	
	format "\tvertexes\n" to:f
	format "\t{\n" to:f
	for tr=1 to trisCount do
	(
		if nmatId != undefined then
		(
			matid = getFaceMatId ms tr
			if matid != nmatId do continue
		)

		local indexes = GetFace ms tr
		local v0p = GetVert ms indexes.x
		local v1p = GetVert ms indexes.y
		local v2p = GetVert ms indexes.z
		
		/*
		local v0n = GetNormal ms indexes.x
		local v1n = GetNormal ms indexes.y
		local v2n = GetNormal ms indexes.z
		*/
		
		local normals = meshop.GetFaceRNormals ms tr

		local v0t
		local v1t
		local v2t
		try
		(
			local texindexes = GetTVFace ms tr
			v0t = GetTVert ms texindexes.x
			v1t = GetTVert ms texindexes.y
			v2t = GetTVert ms texindexes.z
		)
		catch
		(
			v0t = [0, 0]
			v1t = [0, 0]
			v2t = [0, 0]
		)

		format "\t\t! $ { p \"% % %\" n \"% % %\" t \"% %\" }\n" v0p.x v0p.y v0p.z normals[1].x normals[1].y normals[1].z v0t.x v0t.y to:f
		format "\t\t! $ { p \"% % %\" n \"% % %\" t \"% %\" }\n" v1p.x v1p.y v1p.z normals[2].x normals[2].y normals[2].z v1t.x v1t.y to:f
		format "\t\t! $ { p \"% % %\" n \"% % %\" t \"% %\" }\n" v2p.x v2p.y v2p.z normals[3].x normals[3].y normals[3].z v2t.x v2t.y to:f
	)
	format "\t}\n" to:f
)

function printObjectProps obj f = 
(
	if getUserProp obj "NoMesh" == undefined then
		format "\tisMesh 1\n" to:f
	if getUserProp obj "Collision" != undefined then
		format "\tisCollision 1\n" to:f
	
/*	
	format "\tmatrix \"% % % % % % % % % % % %\"\n" obj.transform.row1.x obj.transform.row1.y obj.transform.row1.z obj.transform.row2.x obj.transform.row2.y obj.transform.row2.z obj.transform.row3.x obj.transform.row3.y obj.transform.row3.z obj.transform.row4.x obj.transform.row4.y obj.transform.row4.z to:f
*/
	
	if getUserProp obj "AlphaTest" != undefined then
		format "\tmaterialMethaType \"AlphaTest\"\n" to:f
	if getUserProp obj "Transparent" != undefined then
		format "\tmaterialMethaType \"Transparent\"\n" to:f
	if getUserProp obj "Metal" != undefined then
		format "\tmetal 1\n" to:f
	if getUserProp obj "Frame" != undefined then
		format "\tframe %\n" (getUserProp obj "Frame") to:f	
	if getUserProp obj "FrameCount" != undefined then
		format "\tframeCount %\n" (getUserProp obj "FrameCount") to:f	
	if getUserProp obj "Lod" != undefined then
		format "\tlod %\n" (getUserProp obj "Lod") to:f	
	if getUserProp obj "Id" != undefined then
		format "\tid %\n" (getUserProp obj "Id") to:f	
	if getUserProp obj "TwoSided" != undefined then
		format "\ttwoSided 1\n" to:f
		
	local par = obj.parent
	while ( par != undefined ) do
	(
		if getUserProp par "Bone" != undefined then
			format "\tparent \"%\"\n" par.name to:f	
		par = par.parent
	)
)


rootPath = maxFilePath
if rootPath == undefined then
(
	messageBox "No opened scene"
	exit
)

while rootPath.count > 1 do
(
	sleshIndex = undefined
	for i = 1 to rootPath.count do
		if rootPath[rootPath.count - i + 1] == "\\" then
		( 
			sleshIndex = rootPath.count - i + 1
			exit
		)
	if sleshIndex == undefined then exit
	rootPath = substring rootPath 1 (sleshIndex - 1)
	testFilePath = rootPath + "\\ThisIsRoot"
	if (getfiles testFilePath).count != 0 then exit
)
locName = substring maxFilePath (rootPath.count + 10) maxFilePath.count
locName = locName + maxFileName
locName = substring locName 1 (locName.count - 4)
locName = substituteString locName "\\" "/"

paramFile = openFile (rootPath + "\\meshexp.par") mode: "wt"
format "%" locName to: paramFile
close paramFile

f = createFile (rootPath + "\\meshexp.r") mode: "wt"

for obj in objects do
(
	isMesh = getUserProp obj "NoMesh" == undefined
	isCollision = getUserProp obj "Collision" != undefined
	
	if getUserProp obj "Track" != undefined do 
	(
		local numpoints = numKnots obj
		format "! $ @MeshExpLib::Line\n" to:f
		format "{\n" to:f
		format "\tname \"%\"\n" obj.name to:f
		format "\tvertexes\n" to:f
		format "\t{\n" to:f
		for n=0 to numpoints-1 do
		(
			local t = n as float / numpoints as float
			local pt = pathInterp obj  t
			local ta = pathTangent obj  t
			format "\t\t! $ @MeshExpLib::Line::Vertex { p=\"% % %\" t=\"% % %\" }\n" pt.x pt.y pt.z ta.x ta.y ta.z to:f
		)
		format "\t}\n" to:f
		format "}\n" to:f
	)
	
	if isMesh or isCollision do 
	(
		local ms
		try(
			ms = snapshotAsMesh obj
		)
		catch
		(
			ms = undefined
		)
	
		if ms != undefined then
		(
			if classof obj.material == MultiMaterial  then
			(
				for nsub=1 to obj.material.numsubs do
				(
					format "! $ @MeshExpLib::TriangleGeometry\n" to:f
					format "{\n" to:f
					format "\tname \"%_%\"\n" obj.name nsub to:f
					format "\tmaterial \"%\"\n" obj.material[nsub].name to:f
					printObjectProps obj f
					printVertexArray ms obj f nsub
					format "}\n" to:f
				)
			)
			else
			(
				format "! $ @MeshExpLib::TriangleGeometry\n" to:f
				format "{\n" to:f
				format "\tname \"%\"\n" obj.name to:f
				if obj.material != undefined and obj.material.name != undefined then
					format "\tmaterial \"%\"\n" obj.material.name to:f
				printObjectProps obj f
				printVertexArray ms obj f undefined
				format "}\n" to:f
			)
		)
	)
	
	if getUserProp obj "Bone" != undefined do 
	(
		format "! $ @MeshExpLib::Bone\n" to:f
		format "{\n" to:f
		format "\tname \"%\"\n" obj.name to:f
		printObjectProps obj f
		format "\tmatrix \"% % % % % % % % % % % %\"\n" obj.transform.row1.x obj.transform.row1.y obj.transform.row1.z obj.transform.row2.x obj.transform.row2.y obj.transform.row2.z obj.transform.row3.x obj.transform.row3.y obj.transform.row3.z obj.transform.row4.x obj.transform.row4.y obj.transform.row4.z to:f
		format "}\n" to:f
	)
)

close f

ShellLaunch (rootPath + "//meshexp.exe") ""