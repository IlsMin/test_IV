import QtQuick // 2.15
import QtQuick.Controls //2.15
import QtQuick.Controls.Universal

Button {
//        opacity: 0.7
        //antialiasing: true
        id: control
        height: 30
        width: 80
        enabled: false

/**/
        contentItem: Text {
             text: control.text
             //font: control.font
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
            radius: height/2
            gradient: Gradient
            {

                GradientStop { position: 0.0; color: control.pressed ? Universal.background : Universal.accent}
                GradientStop { position: 0.5; color: control.pressed ? Universal.accent     : Universal.background }
                GradientStop { position: 1.0; color: control.pressed ? Universal.background : Universal.accent }
            }
       }
/**/
}


