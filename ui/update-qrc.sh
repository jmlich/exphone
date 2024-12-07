#!/bin/bash

declare -a platforms=("kirigami" "uuitk" "silica")

for platform in ${platforms[@]}; do

    (
        echo "<RCC>"
        echo "    <qresource prefix=\"/\">"
        replace=platform.${platform}
        for i in $(find ./qml/components/ -path '*platform.'"$platform"'*' -name '*.qml'); do
            x=${i//$replace/platform}; 
            echo "<file alias=\"$x\">$i</file>"; 
        done 
        echo "    </qresource>"
        echo "</RCC>"
    ) > platform-${platform}.qrc

done
