import QtQuick //2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal

Window {
    id: window
    width: 420
    height: 460
    visible: true
    //  Material.accent: Material.BlueGrey

    property int fnt_size :14
    property var mdl_color: ["red","green","blue"]
    property var mdl_state: ["on","off"]
    property string cmd: ""
    property bool is_setter: true

    function onSocketError(msg : string)
    {
        t_state.text += msg;
        if(!msg.endsWith('\n'))
          t_state.text += '\n';
        //TODO some error indicator

      //  scrollView.ScrollBar.vertical.  position = 1.0;
      t_state.cursorPosition = t_state.length-2;
    }

    function onSocketState(msg : string, state: int)
    {
        onSocketError(msg);
        btn_send.enabled = state === 3;//QAbstractSocket::ConnectedState
    }

    function getCurrentCmd()
    {
       is_setter = r_setter.checked;
       cmd =  is_setter? "s":"g" ;
       cmd += "et-led-" + cb_cmnds.currentText + " ";

         if (is_setter)
         {
            if (r_hardVal.checked)
                  cmd += cb_val.currentText;
             else cmd += e_val.text;
         }
         cmd += "\n";
         console.log(cmd);
       return cmd;
    }

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
            anchors.rightMargin: 12
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
            }

            VP_TextEdit {
                id: e_port
                text: qsTr("12345")
            }
        }


    }

    GroupBox {
        id: groupBox1
        x: 25
        y: 131
        width: 273
        height: 195
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
            }


            ComboBox {
                id: cb_cmnds
                currentIndex: -1
                model: ["state","rate","color"]
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

            VP_TextEdit {
                id: e_val
                text: qsTr("some_value")
                readOnly: true

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
            //clientSocket.
            if(autoClear.checked)
                t_state.text = "";

            clientSocket.sendToServer(getCurrentCmd());
        }


    }

    VP_Button {
        id: vP_Button
        x: 312
        y: 84
        width: 80
        text: "Connect"
        enabled:  true;
        onClicked: clientSocket.init(e_ip.text.trim(), parseInt(e_port.text))
    }

    VP_Button {
        id: btn_send1
        x: 312
        y: 412
        width: 80
        text: qsTr("Clear")
        enabled: true
        onClicked: t_state.text = "";
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
        hoverEnabled: true
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
        title: qsTr("socket state/server answer")
        x: 25
        y: 344
        width: 273
        height: 98
        anchors.leftMargin: 24

        ScrollView {
            id: scrollView
            anchors.fill: parent
            //padding: 0
            TextArea {
              id: t_state
               // anchors.fill: parent
              readOnly: true
              background: Rectangle { border.color: "transparent" }
            }
        }
    }
    
}


