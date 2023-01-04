import QtQuick //2.15
//import QtQuick.Controls 1.0
//import QtQuick.Controls// 2.15
import QtQuick.Controls // 2.15
import QtQuick.Controls.Universal
//import QtQuick.Controls.Styles 1.0
//import QtQuick.Controls.macOS 6.0


Window {
    id: window
    width: 420
    height: 460
    visible: true
    //  Material.accent: Material.BlueGrey

    property int fnt_size :14
    property var mdl_color: ["red","green","blue"]
    property var mdl_state: ["on","off"]

    title: qsTr("test_IV  client")
    GroupBox {
        id: groupBox
        x: 25
        width: 273
        height: 90
        anchors.top: parent.top
        anchors.topMargin: 24
        title: qsTr("Server  socket  info")

        Grid {
            id: grid2
             anchors.fill: parent
            anchors.leftMargin: 12
            anchors.bottomMargin: 12
            anchors.rightMargin: 5
            flow: Grid.LeftToRight
            spacing: 12
            rows: 2
            columns: 2


            Text {
                id: t_ip
                text: qsTr("   IP:")

            }

            VP_TextEdit {
                id: e_ip

                // text: "192.168.1.2"
                text: ("127.0.0.1")
            }


            Text {
                id: t_port
                text: qsTr("Port:")
                // font.pixelSize: fnt_size
            }


            VP_TextEdit {
                id: e_port

                text: qsTr("12345")
                // font.pixelSize: fnt_size
                //onTextChanged: listenPort();
            }
        }


    }

    GroupBox {
        id: groupBox1
        x: 25
        y: 135
        width: 273
        height: 191
        title: qsTr("command parameters")

        Grid {
            id: grid1
            anchors.fill: parent
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter
            flow: Grid.LeftToRight
          //  rightPadding: 8
            //   anchors.rightMargin:  7
            rows: 0
            spacing: 2
            columns: 2
            //1 -st row
            Text {
                id: t_type

                text: qsTr("type :")

           }

            Row {
                id: row
                //width: 200
               // height: 40

                RadioButton {
                    id: r_setter
                    text:"setter"
                    checkable: true
                    checked:  true

                }

                RadioButton {
                    id: r_getter
                    //   width: cb_cmnds.width
                    text:"getter"
                    //    anchors.right: parent.right
                    //  anchors.rightMargin: 6
                    onCheckedChanged: //StateChanged:
                    {
                        var enabl =  !checked
                        cb_val.enabled    = enabl;
                        e_val.enabled     = enabl;
                        r_freeVal.enabled = enabl;
                        r_hardVal.enabled = enabl;
                        if(enabl)
                        { // call  r_freeVal.onCheckedChanged ?
                            cb_val.enabled = !r_freeVal.checked;
                            e_val. enabled =  r_freeVal.checked;
                            e_val.readOnly = !e_val.enabled;

                        }

                    }
                }
            }
            //2-nd row
            Text {
                         id: t_cmd

                         text: qsTr("command :")
                        // anchors.verticalCenter: cb_cmnds.verticalCenter
                         //anchors.verticalCenter: parent.verticalCenter
                         //   font.pixelSize: fnt_size
                     }


            ComboBox {
                id: cb_cmnds
                currentIndex: -1
                model: ["state","rate","color"]
                //  model: ["red","green","blue"]
                Component.onCompleted: currentIndex =  0;
                onCurrentIndexChanged:
                {
                    switch(currentIndex)
                     { case 0:  cb_val.model = mdl_state; break;
                       case 1:  cb_val.model = 6;         break;
                       case 2:  cb_val.model = mdl_color; break;
                     }
                }
            }

            //3 row
            Text {text: " value ";  font.underline: true}
            Text {text: "             "}
            RadioButton {
                       id: r_hardVal

                         text: qsTr("predefined:")
                         checked:  true

                  }

            ComboBox {
                id: cb_val
                //  width: 99
                // contentItem: {"red","green","blue"}

               // model: md_color
                //model: ["red","green","blue"]   model: ["red","green","blue"]


            }
            //3 row
            RadioButton {
                          id: r_freeVal

                         text: qsTr("free/edited:")
                         onCheckedChanged: //StateChanged:
                         {
                             cb_val.enabled = !checked;
                             e_val. enabled =  checked;
                             e_val.readOnly = !e_val.enabled;
                           }
                    }

            TextEdit {
                id: e_val
                text: qsTr("some_value")
                readOnly: true
                // font.pixelSize: fnt_size
                font.bold: cursorVisible
            }

        }
    }

    VP_Button {
        id: btn_send
        x: 312
        y: 296
        width: 80
        text: qsTr("Send")
        onClicked:
        {
            //if(socket.active) wait it
            socket.active = false
            socket.url =  "ws://" + e_ip.text.trim();// + ":"+e_port.text.trim()
            //socket.url = "ws://echo.websocket.org"
            console.log(socket.url)
            socket.active = true
        }


    }

    VP_Button {
        id: vP_Button
        x: 312
        y: 84
        width: 80
        text: "Connect"
        enabled:  true;
        // onClicked: Vibrator.test()
    }

    VP_Button {
        id: btn_send1
        x: 312
        y: 412
        width: 80
        text: qsTr("Clear")
    }
    CheckBox {
        id: autoClear
        x: 311
        y: 366
        width: 95
        height: 28
        text: "auto-clear"
        HoverHandler {
                  id: hoverHandler
                }
        ToolTip  {
            id: toolTip
            visible: autoClear.hovered
            timeout: 1800
            text: "clear server answer on sending new command"
          //  x:  autoClear.left// hoverHandler.point.position.x// -height
            y: hoverHandler.point.position.y - autoClear.height
         }
        checked: true

        //     indicator.width: 28
        //   indicator.height: 28
        anchors.left: groupBox2.right
        clip: false
        anchors.leftMargin: 14
        hoverEnabled: false
        padding: 0
        bottomPadding: 0
        topPadding: 0
        checkState: Qt.Checked
        tristate: false
        checkable: true
        enabled: true
    }
    GroupBox {
        id: groupBox2
        title: qsTr("server answer")
        x: 25
        y: 344
        width: 273
        height: 98
        anchors.leftMargin: 24
        Grid {
            id: grid3
            anchors.fill: parent
            anchors.bottomMargin: 0
            anchors.leftMargin: 24
            anchors.topMargin: 24
            // anchors.bottomMargin: 87
            spacing: 8
            columns: 2

            Text {
                id: t_state
                width: 273
                height: 30
                text: qsTr("")
                // anchors.bottom: parent.bottom
                font.pixelSize: 12
                anchors.bottomMargin: 44
            }
        }
    }
    
}
