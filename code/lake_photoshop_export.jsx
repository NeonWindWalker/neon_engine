
main();

function saveLayer( doc, layerName, fileName, depth )
{
    var layerFinded = false;
    for( var i = 0; i < doc.layers.length; i++)
    {
		var name = doc.layers[i].name;
		if(name === layerName)
		{
			doc.layers[i].visible = true;
            layerFinded = true;
		}
		else
		{
			 doc.layers[i].visible = false;
		}
    }

    if(!layerFinded)
	{
		var delFile = new File(fileName);
		delFile.remove();
		return;
	}
    
    var saveFile = new File(fileName);
    bmpSaveOptions = new BMPSaveOptions();
    bmpSaveOptions.depth = depth;
	bmpSaveOptions.flipRowOrder = false;
    doc.saveAs(saveFile, bmpSaveOptions, true, Extension.LOWERCASE);
}

function layerOrGroupExists( doc, layerName )
{
    for( var i = 0; i < doc.layers.length; i++)
        if(doc.layers[i].name == layerName)
            return true;
			
	for( var i = 0; i < doc.layerSets.length; i++)
        if(doc.layerSets[i].name == layerName)
            return true;
            
    return false;
}

function mergeLayerSet( leyerSet )
{
    while(leyerSet.layerSets.length > 0)
		mergeLayerSet(leyerSet.layerSets[0]);
            
    leyerSet.merge();
}

function main()
{
    if ( app.documents.length <= 0 )
    	return 'cancel';

    var psdFile = new File(app.activeDocument.fullName);
    var rootDir = psdFile.parent;
    var rootPath = "";
    while(rootDir && rootDir.exists)
    {
        var file = new File(rootDir.path + "/ThisIsRoot");
        if(file.exists){
            rootPath = rootDir.path;
            break;
         }
        rootDir = rootDir.parent;
    }
    
    if(rootPath == "")
        return 'cancel';
    var locFileName = psdFile.fullName.substring(rootPath.length + 9);
    locFileName = locFileName.substring(0, locFileName.length - 4);
	
	var psdPathBase = psdFile.fullName.substring(0, psdFile.fullName.length - 4);
	
	var duppedDocument = app.activeDocument.duplicate();
	
    if( layerOrGroupExists( duppedDocument, "Albedo") )
    {
		for( var i = 0; i < duppedDocument.layerSets.length; )
		{	
			var name = duppedDocument.layerSets[i].name;
			if( name === "Albedo" || name === "Opacity" || name === "Normal" || name === "Specular" || name === "Glossines" || name === "Frensel" || name === "Metal" || name === "SSS" )
			{
				mergeLayerSet(duppedDocument.layerSets[i]);
				i = 0;
			}
			else
			{
				duppedDocument.layerSets[i].visible = false;
				i++;
			}
		}
	
		duppedDocument.bitsPerChannel = BitsPerChannelType.EIGHT;
	
        saveLayer( duppedDocument, "Albedo",  psdPathBase + "_albedo.bmp", BMPDepthType.TWENTYFOUR );
		saveLayer( duppedDocument, "Opacity",  psdPathBase + "_opacity.bmp", BMPDepthType.TWENTYFOUR );
        saveLayer( duppedDocument, "Normal",  psdPathBase + "_normal.bmp", BMPDepthType.TWENTYFOUR );
        saveLayer( duppedDocument, "Specular",  psdPathBase + "_specular.bmp", BMPDepthType.TWENTYFOUR );
        saveLayer( duppedDocument, "Glossines",  psdPathBase + "_glossines.bmp", BMPDepthType.TWENTYFOUR );
		saveLayer( duppedDocument, "Frensel",  psdPathBase + "_frensel.bmp", BMPDepthType.TWENTYFOUR );
        saveLayer( duppedDocument, "Metal",  psdPathBase + "_metal.bmp", BMPDepthType.TWENTYFOUR );
		saveLayer( duppedDocument, "SSS",  psdPathBase + "_sss.bmp", BMPDepthType.TWENTYFOUR );
    }
    else
    {
		while(duppedDocument.layerSets.length > 0)
			mergeLayerSet(duppedDocument.layerSets[0]);
		
		duppedDocument.bitsPerChannel = BitsPerChannelType.EIGHT;
	
        var saveFile = new File(psdPathBase + ".bmp");
        bmpSaveOptions = new BMPSaveOptions();
        bmpSaveOptions.depth = BMPDepthType.THIRTYTWO;
		bmpSaveOptions.flipRowOrder = false;
        duppedDocument.saveAs(saveFile, bmpSaveOptions, true, Extension.LOWERCASE);
    }
	
	duppedDocument.close( SaveOptions.DONOTSAVECHANGES );

    var params = new File(rootPath + "/texexp.par");
    params.open("w");
    params.write(locFileName);
    params.close();
    
    var exporter = new File(rootPath + "/texexp.exe");
    exporter.execute();
}




///////////////////////////////////////////////////////////////////////////////
// Function: initExportInfo
// Usage: create our default parameters
// Input: a new Object
// Return: a new object with params set to default
///////////////////////////////////////////////////////////////////////////////
function initExportInfo(exportInfo) {
    
    try {
        exportInfo.destination = Folder(app.activeDocument.fullName.parent).fsName; // destination folder
        var tmp = app.activeDocument.fullName.name;
        exportInfo.fileNamePrefix = decodeURI(tmp.substring(0, tmp.indexOf("."))); // filename body part
    } catch(someError) {
        exportInfo.destination = new String("");
        exportInfo.fileNamePrefix = app.activeDocument.name; // filename body part
    }
}