import QtQuick 2.1
import QtQuick.Controls 2.1

Button {
//        opacity: 0.7
        //antialiasing: true
        id: control
        height: 30
        width: 80

        contentItem: Text {
             text: control.text
             font:  control.font
             opacity: enabled ? 1.0 : 0.3
             horizontalAlignment: Text.AlignHCenter
             verticalAlignment:   Text.AlignVCenter
             elide: Text.ElideRight
         }
        background: Rectangle
        {
            //opacity: enabled ? 1 : 0.3
            border.color: "#aaaaaa"
            border.width: 1
            radius: 5
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: control.pressed ? "gray": "white" }
                GradientStop { position: 1.0; color: control.pressed ? "white": "gray" }
            }
       }

}


