OBJECTIVE:	Write client-server chat application program that can handle multiple clients at a time.

PROTOCOLS DEFINED: :) One child process for handling each client is created on server side.
		   :) Each child process will further split into two process. First process reads data from clients and sends it to parent, 			      and another process receive data from parent and sends it to client.
		   :) Routing is done by parent process, he will check the dstn addr of data and sends the data to the child process handling
		      the client having that dstn addr.
		   :) Every data transfer contains src addr, dstn addr, and protocl used.
		   :) src addr is client id of client that sends msg, and dstn addr is client id for whom msg is sent.
		   :) client id is always >= 1. Each client will be assigned a client id by server which is >=1.
		   :) server addr is -1.	If some msg is send to server dstn addr should be -1.
		   :) broadcast addr is 0.	If you want to broadcast a msg, set the dstn addr to 0, server will broadcast it.
		   :) protocol: 0 => msg is either unicast(with dstn addr = client id)/broadcast(with dstn addr = 0).
		   :) protocol: 1 => used by client with dstn addr -1 to get list of clients online.
		   :) protocol: 2 => used by client with dstn addr -1 to change the client name.
		   :) protocol: 3 => used by child processes of server with dstn addr -1 incase client gets disconnected. Child process sends 					     this msg to parent and child process exits. Parent process recives the msg and remove the records of that 					     client. It updates the information about clients online at present time.
		   :) protocol: 4 => unsed....
		   :) protocol: 5 => broadcast for new client connection.
