 

 
 var state = true
 var killTask = -1

 let socket = new WebSocket("ws://127.0.0.1:8080/ws");
        console.log("Attempting Connection...");

        socket.onopen = () => {
            console.log("Successfully Connected");
            socket.send("-1")
        };
        
        socket.onmessage = (msg) => {
            setTimeout(function() {
                var fullObj = JSON.parse(msg.data); 
                console.log(fullObj);

                obj = fullObj.cpu

                var rTaskNode = document.getElementById('totalRam')
                rTaskNode.innerHTML = fullObj.ram.total * 4 / 1024

                var rTaskNode = document.getElementById('freeRam')
                rTaskNode.innerHTML = fullObj.ram.libre * 4 / 1024

                var rTaskNode = document.getElementById('runningRam')
                rTaskNode.innerHTML = (fullObj.ram.total - fullObj.ram.libre) * 4 / 1024

          


      
                
                var rTaskNode = document.getElementById('rTask')
                rTaskNode.innerHTML = obj.procCount.procX

                var rTaskNode = document.getElementById('sTask')
                rTaskNode.innerHTML = obj.procCount.procS

                var rTaskNode = document.getElementById('dTask')
                rTaskNode.innerHTML = obj.procCount.procD

                var rTaskNode = document.getElementById('zTask')
                rTaskNode.innerHTML = obj.procCount.procZ

                var tbodyRef = document.getElementById('cpuTable')
                let stringContent = ''
                let totalRam = 0
                for(n of obj.procList){
                    if(isNaN(n.ramm)) continue
                    totalRam += n.ramm
                }
                for(node of obj.procList){
                    
                    if(node.ID == undefined) continue
                    
                    stringContent += `<tr data-toggle="collapse"  data-target="#n${node.ID}" data-parent="#myTable" onclick="changeState()">`
                    stringContent += '\t<td>'
                    stringContent += node.ID 
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += node.Name
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += node.User == 0 ? "root" : node.User == 1000 ? "samuelr+" : node.User == 133 ? "rtkit" : node.User == 977 ? "avahi" :  node.User == 81 ? "dbus" :node.User == 102 ? "polkitd" :node.User == 87 ? "ntp" :
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += getState(node.State)
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += node.ramm / 5862 / 1024 * 100
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += node.ramm / totalRam  * 100
                    stringContent += '\t</td>'
                    stringContent += '\t<td>'
                    stringContent += ` <button type="button" class="btn btn-dark" id="btn${node.ID}"   value="${node.ID}" >Kill</button> `
                    stringContent += '\t</td>'
                    stringContent += '</tr>'

                    for(child of node.childs){
                       if(child.ID == undefined) continue 
                        stringContent += `<tr id="n${node.ID}" class="collapse">`
                        stringContent += '\t<td class="hiddenRow">'
                        stringContent += child.ID 
                        stringContent += '\t</td>'
                        stringContent += '\t<td class="hiddenRow">'
                        stringContent += child.Name
                        stringContent += '\t</td>'
                        stringContent += '</tr>'

                    }

                }

                if(state) tbodyRef.innerHTML = stringContent

                
                for(node of obj.procList){
                    if(node.ID == undefined) continue
                    let aButton = document.getElementById(`btn${node.ID}`)
                    aButton.addEventListener('click',function(){ getSelection(this)},false);
                }


            
                    socket.send("-1")
                
            
            }, 500);
          
        }

        socket.onclose = event => {
            console.log("Socket Closed Connection: ", event);
            socket.send("Client Closed!")
        };

        socket.onerror = error => {
            console.log("Socket Error: ", error);
        };


        function getState(st){
            if(st == 0){
                return 'Running'
            }else if(st == 1 || st == 1026){
                return 'Sleeping'
            }else if(st == 128){
                return 'Deleted'
            }else if(st == 260){
                return 'Zombie'
            }

        }

        function changeState() {
            state = !state
        }

        function getSelection(sel){
            var value = sel.value;
            alert(value + "Killed")
            socket.send(value)
        }

