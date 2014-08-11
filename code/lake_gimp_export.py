#!/usr/bin/env python

import gimpfu as mod_gimpfu
import re as mod_re
import sys
import os
import os.path

def save_layers( image, layer ):
	isLayer = False
	basePath = mod_gimpfu.pdb.gimp_image_get_filename(image)
	basePath = basePath[:basePath.rfind(".")]
	for lr in image.layers:
		name = None
		if lr.name == "Albedo":
			name = "albedo"
		if lr.name == "Opacity":
			name = "opacity"
		if lr.name == "Specular":
			name = "specular"
		if lr.name == "Glossines":
			name = "glossines"
		if lr.name == "Normal":
			name = "normal"
		if lr.name == "Frensel":
			name = "frensel"
		if lr.name == "Metal":
			name = "metal"
		if lr.name == "SSS":
			name = "sss"
			
		if name != None:
			name = basePath + "_" + name + ".bmp"
			isLayer = True
			mod_gimpfu.pdb.gimp_file_save( image, lr, name, name )
	
	if isLayer == False:
		name = basePath + ".bmp"
		mod_gimpfu.pdb.gimp_file_save( image, layer, name, name )
	
	path = mod_gimpfu.pdb.gimp_image_get_filename(image)
	path = os.path.dirname(path)
	oldpath = ""
	while path != None and path != "" and path != oldpath :
		if os.path.exists(path + "/ThisIsRoot") == True :
			#mod_gimpfu.pdb.gimp_message(path)
			fd = os.open(path + "/texexp.par", os.O_WRONLY | os.O_CREAT | os.O_TRUNC )
			locPath = basePath[len(path) + 9:]
			os.write(fd, locPath)
			os.close(fd)
			os.system(path + "/texexp.exe")
			break
		oldpath = path
		path = os.path.dirname(path)
		

mod_gimpfu.register(
        "texture_export",
        "Export To Textures",
        "Export To Textures",
        "Shutov N.V.",
        "Shutov N.V.",
        "2013",
        "<Image>/Lake/_Save Textures",
        "RGB*, GRAY*",
        [],
        [],
        save_layers )

mod_gimpfu.main()