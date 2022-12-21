import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
//import WebSockets

Window
{
    id: window
    width: 400
    height: 600
    visible: true
    title: qsTr("IVideon test client");
    minimumWidth: 400
    minimumHeight: 250
    //property int radius: 4
    property int fnt_size: 14
    property int txt_width: 100


    GroupBox {
        id: groupBox1
        x: 35
        y: 160
        width: 260
        height: 122
        title: qsTr("Command  parameters")

        TextEdit {
            id: e_cmnd
            x: -6
            y: 10
            width: 247
            height: 100
            text: qsTr("type test
command here")
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }
    }

    GroupBox {
        id: groupBox
        x: 31
        y: 27
        width: 268
        height: 104
        title: qsTr("Partner  socket  info")

        Grid {
            id: grid2
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 8
            spacing: 8
            anchors.fill: parent
            rows: 2
            columns: 2














            Text {
                id: element2
                text: qsTr("   IP:")
                horizontalAlignment: Text.AlignHCenter
                width: txt_width
            }

            VP_TextEdit {
                id: e_ip
                width: 80
                height: 20
                text: "192.168.1.2"
                //text: qsTr("100.64.0.1")
                //                text: qsTr("127.0.0.1")
            }

            Text {
                width: txt_width
                text: qsTr("Port:")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: fnt_size
            }
            TextEdit {
                id: e_port
                width: 80
                height: 20
                text: qsTr("12345")
                font.pixelSize: fnt_size
                font.bold: cursorVisible
                //onTextChanged: listenPort();
            }
        }
    }

    VP_Button {
        id: btn_send
        x: 305
        y: 252
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

    GroupBox {
        id: groupBox2
        x: 35
        y: 316
        width: 260
        height: 122

        TextArea {
            id: t_response
            x: 3
            y: 19
            width: 233
            height: 91
            placeholderText: qsTr("Text Area")
        }
        title: qsTr("Server responses")
    }

    VP_Button {
        id: btn_send1
        x: 305
        y: 408
        width: 80
        text: qsTr("Clear")
    }

    VP_Button {
        id: btn_send2
        x: 305
        y: 101
        width: 80
        text: qsTr("Connect")
    }

    CheckBox {
        id: checkBox
        x: 238
        y: 288
        width: 147
        height: 40
        text: qsTr("Clear on send")
        anchors.right: parent.right
        anchors.rightMargin: 15
        transformOrigin: Item.Right
        layer.textureMirroring: ShaderEffectSource.NoMirroring
    }
    /** /
    WebSocket
    {
        id: socket
        //active: true

        onTextMessageReceived:
        {
            t_state.text ="Server message: " + message

        }
        onStatusChanged:
        {
            if (socket.status === WebSocket.Error)
                 t_state.text = "Error: " + socket.errorString
              else
             if (socket.status === WebSocket.Open)
             {
                 var mes = "{\"len\":%1, \"freq\":%2}"
                 t_state.text = "Sending to "+socket.url
                 mes = mes.arg(e_time.text.trim());
                 mes = mes.arg(e_freq.text.trim());
                 socket.sendTextMessage(mes)
             } else
             if (socket.status === WebSocket.Closed)

                 t_state.text = "Socket closed"
              else
             if (socket.status === WebSocket.Connecting)
                 t_state.text = "Connecting..."


        console.log(t_state.text)
        }
    }
    /** /
    WebSocketServer
    {
              id: server
              //port: e_port.text.trim()
              //listen: false
              //accept: true
              onClientConnected:
              {
                  t_state.text ="onClientConnect"
                  webSocket.onTextMessageReceived.connect(function(message)
                  {
                      t_state.text ="Client message: " + message
                      var jsonObj= JSON.parse(message);
                      var len  = jsonObj.len;
                      var freq = jsonObj.freq;
                      if (freq >0 && freq <256 && len >0 && len <=10000)
                           Vibrator.vibrate(len, freq);
                  });
              }
              onErrorStringChanged:
              {
                  t_state.text = "Error: " + socket.errorString
              }
               Component.onCompleted:
                  listenPort();
     }
/**/
// https://codeblog.vurdalakov.net/2009/11/solution-qsslsocket-cannot-call.html
// https://forum.qt.io/topic/91777/qsslsocket-cannot-call-unresolved-function-sslv23_client_method/2
// https://bintray.com/vszakats/generic/openssl
    function listenPort()
    {
        //console.log(socket.errorString)
        server.port = e_port.text.trim()
        server.listen = true
        server.accept = true
        //console.log(socket.errorString)
        if (server.listen === true)
            t_state.text = server.url  + " port listening..."
        else
            t_state.text = "server not started"

    }
//    WebSocketServer{

//            property var listSockets: []

//            id: idWebSocketServer
//            accept: true
//            listen: true
//            host: "127.0.0.1"
//            port: 12345

//            onClientConnected: {

//                var _socketName = webSocket.toString()

//                if(!listSockets[_socketName]){

//                    listSockets[_socketName] = webSocket
//                    listClientURL.push(url)
//                }

//                var _socket = webSocket.onTextMessageReceived.connect(function(message) {

//                    for (var i in listSockets){

//                        if(listSockets[i].status == WebSocket.Open){

//                            listSockets[i].sendTextMessage(message)
//                        }
//                    }
//              });
//            }

//            onErrorStringChanged: {

//               t_state.text = errorString
//                console.log("Ошибка: " + errorString)
//            }
//            Component.onCompleted: {
    //                t_state.text = url+ "_" + name
////                        wsRoot.appendMessage("Server URL: " + url);
//                        // activate the client connection
//                        //clientSocket.url = url;
//                    }
//        }
    Component.onCompleted:
    {
        //listenPort();
    }
}

