#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Usage: encoder <folder> <type> <search string> <replace string>"
fi

DIR="$1"
TYPE3D="$2"
PATTERN=$3
ReplaceString=$4


EncodeReplacePath() {
    echo "EncodeReplacePath"
    for fname in $(find $DIR -name '*.mtl');
    do
        if [[ "$fname" =~ $PATTERN ]]; then
           echo  "Pattern Found "
	   result=$(grep -i "newmtl" $fname | awk '{ print $2 }')
	   normalText=$result
	   normalText+="_normal.png"
           echo $normalText
	   diffuseText=$result
	   diffuseText+="_diffuse.png"
	   echo $diffuseText

           if [[ "${ReplaceString}" =~ "ALL" ]] ;then
	   # if grep -q "ALL" $ReplaceString; then
              # construct the Normal Replace String
	       normalfilename="${ReplaceString%.*}"
               normalfilename="${normalfilename/"ALL"/"normal"}"
	       normalfilename+=".png"
	       echo "####Normal Texture: $normalfilename ###"
	       diffusefilename="${ReplaceString%.*}"
               diffusefilename="${diffusefilename/"ALL"/"diffuse"}"
	       diffusefilename+=".png"
	       echo "####Diffuse Texture: $diffusefilename ###"

	       $(sed -i "s/${normalText}/${normalfilename}/g" ${fname})
	       $(sed -i "s/${diffuseText}/${diffusefilename}/g" ${fname})
               #echo "sed -i "s/${normalText}/${normalfilename}/g" ${fname}"
               #echo "sed -i "s/${diffuseText}/${diffusefilename}/g" ${fname}"


           else
                echo "TODO : Use the Supplied Replace Pattern" 
               #echo "sed -i "s/${normalText}/${normalfilename}/g" ${fname}"
           fi

	  # echo "####MatchString### $result"
          # echo "sed -i "s/${normalText}/${ReplaceString}/g" ${fname}"
          # echo "sed -i "s/${diffuseText}/${ReplaceString}/g" ${fname}"

	  # echo $fname
	 fi
    done;

}

RenameTexture() {
    echo "Rename Texture"
    SearchExt="*.png"
    for fname in $(find $DIR -name "$SearchExt");
    do
        echo $fname
	filename=$(basename "$fname")
        extension="${filename##*.}"
	echo $extension
        if test "$extension" == "png"; then
            SearchExt="*.png"
	     if [[ "$fname" =~ $PATTERN ]]; then
                echo  "Pattern Found "
		newTexFullName=$(dirname "$fname")
		newTexFullName+="/"
		newTexFullName+=$ReplaceString
                echo "mv $fname $newTexFullName"
                mv $fname $newTexFullName
	     else
		echo "Pattern Not Found"
	     fi
         fi


    done;

}


Rename() {
    echo "Rename Function"
    SearchExt=""
    echo $1 $2 $3 $4
    FileExt=$3
    ObjType=""
    if test "$FileExt" == ".obj"; then
	    ObjType="obj"
	    SearchExt="*.obj"
    fi

    if test "$FileExt" == ".fbx"; then
	    objType="fbx"
	    SearchExt="*.fbx"
    fi

    if test "$FileExt" == ".glb"; then
	    objType="glb"
	    SearchExt="*.glb"
    fi

    if test "$FileExt" == ".mtl"; then
	    objType="mtl"
	    SearchExt="*.mtl"
    fi

    if test "$FileExt" == ".png"; then
	    objType="mtl"
	    SearchExt="*.png"
    fi
    
    # echo $DIR $FileExt

    for fname in $(find $DIR -name "$SearchExt");
    do
	    echo $fname
            if [[ "$fname" =~ $PATTERN ]]; then
                echo  "Pattern Found "
                if [[ "${fname}" =~ "HI" ]] ;then
                        NewFile=""
                        echo $fname
			Origfilename=$(basename "$fname")
                        dir=$(dirname "$fname")
                        echo "Directory $dir"
                        # construct the file name from the replace string given
                        NewFile=$ReplaceString
                        NewFile+="_mesh_"
			NewFile+=$ObjType
			NewFile+="_high_"
                        NewFile+=$PATTERN
                        NewFile+=$FileExt
                        dir+="\\"
                        dir+=$NewFile
                        echo "New File $dir"
		        # For OBJ we need to rename the mtl file as well, so a speacial handeling is needed for mtl
			if test $FileExt==".obj" && test "$TYPE3D" == "OBJ"
			then
			    origMTL=$(dirname "$fname")
			    NewMTLDir=$(dirname "$fname")
			    filename=$(basename "$fname")
			    filename="${filename%.*}"
			    filename="${filename/"obj"/"mtl"}"
			    filename+=".mtl"
			    # echo "#### $filename #####"
			    mtlfilename+=$(dirname "$fname")
			    mtlfilename+=filename
			    NewMTLFile=$ReplaceString
                            NewMTLFile+="_mesh_mtl"
                            NewMTLFile+="_high_"
                            NewMTLFile+=$PATTERN
                            NewMTLFile+=".mtl"
			    origMTL+="\\"
                            origMTL+=$filename
			    NewMTLDir+="\\"
			    NewMTLDir+=$NewMTLFile
			    echo "Move ###MTL### $origMTL $NewMTLDir ###"
			    mv $origMTL $NewMTLFile

			fi
                        echo "Move ###OBJ### $fname $dir ####"
                         mv $fname $dir
                fi

                if [[ "${fname}" =~ "LO" ]] ;then
                        NewFile=""
                        echo $fname
                        dir=$(dirname "$fname")
                        echo "Directory $dir"
                        # construct the file name from the replace string given
                        NewFile=$ReplaceString
                        NewFile+="_mesh_"
			NewFile+=$ObjType
			NewFile+="_low_"
                        NewFile+=$PATTERN
                        NewFile+=$FileExt
                        dir+="\\"
                        dir+=$NewFile
                        echo "New File $dir"

		        # For OBJ we need to rename the mtl file as well, so a speacial handeling is needed for mtl
			if test $FileExt==".obj" && test "$TYPE3D" == "OBJ"
			then
			    origMTL=$(dirname "$fname")
			    NewMTLDir=$(dirname "$fname")
			    filename=$(basename "$fname")
			    filename="${filename%.*}"
			    filename="${filename/"obj"/"mtl"}"
			    filename+=".mtl"
			    # echo "#### $filename #####"
			    mtlfilename+=$(dirname "$fname")
			    mtlfilename+=filename
			    NewMTLFile=$ReplaceString
                            NewMTLFile+="_mesh_mtl"
                            NewMTLFile+="_low_"
                            NewMTLFile+=$PATTERN
                            NewMTLFile+=".mtl"
			    origMTL+="\\"
                            origMTL+=$filename
			    NewMTLDir+="\\"
			    NewMTLDir+=$NewMTLFile
			    echo "Move ###MTL### $origMTL $NewMTLDir ###"
			    mv $origMTL $NewMTLFile

			fi
                        mv $fname $dir
                fi
            else
                echo "Pattern Not Found for $fname"

            fi
            # if the file is in HI folder rename accordingly
    done;

}

if [ ! -d $DIR ] 
then
    echo "Directory $DIR DOES NOT exists." 
    exit 9999 # die with error code 9999
fi

if  test "$TYPE3D" != "OBJ"   &&  test "$TYPE3D" != "GLB" &&  test "$TYPE3D" != "FBX" && test "$TYPE3D" != "TEX" && test "$TYPE3D" != "MTL"
then
	echo "Type can be wither OBJ or GLB or FBX or TEX or MTL: $TYPE3D"
fi

if test "$TYPE3D" == "FBX"
then
    echo "Type is FBX"
    for fname in $(find $DIR -name '*.fbx');
    do
	    if [[ "$fname" =~ $PATTERN ]]; then
                echo  "Pattern Found "
		if [[ "${fname}" =~ "HI" ]] ;then
			NewFile=""
			echo $fname
			dir=$(dirname "$fname")
			echo "Directory $dir"
			# construct the file name from the replace string given
			NewFile=$ReplaceString
			NewFile+="_mesh_fbx_high_"
			NewFile+=$PATTERN
			NewFile+=".fbx"
			dir+="\\"
			dir+=$NewFile
			echo "New File $dir"
			mv $fname $dir
		fi
		if [[ "${fname}" =~ "LO" ]] ;then
			NewFile=""
			echo $fname
			dir=$(dirname "$fname")
			echo "Directory $dir"
			# construct the file name from the replace string given
			NewFile=$ReplaceString
			NewFile+="_mesh_fbx_low_"
			NewFile+=$PATTERN
			NewFile+=".fbx"
			dir+="\\"
			dir+=$NewFile
			echo "New File $dir"
			mv $fname $dir
		fi
            else
                echo "Pattern Not Found for $fname"
		
            fi
	    # if the file is in HI folder rename accordingly
    done;
fi

if test "$TYPE3D" == "OBJ"
then
	EXT=".obj"
	Rename $DIR $PATTERN $EXT $ReplaceString
fi


if test "$TYPE3D" == "MTL"
then
	EXT=".mtl"
	EncodeReplacePath $DIR $PATTERN $ReplaceString
fi

if test "$TYPE3D" == "GLB"
then
	EXT=".glb"
	Rename $DIR $PATTERN $ReplaceString
fi

if test "$TYPE3D" == "TEX"
then
	EXT=".png"
	RenameTexture $DIR $PATTERN $ReplaceString
fi



