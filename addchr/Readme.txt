OBJECTIVE:      Write a client server application program in which client takes input a string from user and sends it to server. 
		Server checks if string contains only digits then sends the sum of digits in that string, else sends "sorry" to client.
		Client continues to send the received string to server untill it is not a single digit number.

PROCEDURE:      #Client Side.......
		Client scans a string from user and sends it to server.
		Client reads string from server. If string contains "sorry" it prints "sorry" and exit.
		Else untill string size is equal to 1, client sends the received string to server and again reads from server.

		#Server Side.......
		Server reads a string from client, if it contains any non-digit character it sends "sorry" to client, else sends sum of digit.
		While sum of digits >= 10 reads string from client, computes sum of digits in string, sends the sum to client.

RESULT:		Input: 123567832387439345678
		Output: 88
			16
			7


