The purpose of this project was to design and implement a simple database that will allow us to keep track of our customers and their information that we know about them. I would need to build a simple interface around this database that will allow me to read a database file from the disk, add customers, delete cuustomers, and save the database back to the file. I would need to store these customers into a hash table

# typedef struct

For the struct I made it similar to the one that was in the demos that Alex did for hashtables but I just needed to add more values. The email would be the key and the name, shoe size, and food would be the values.

```
typedef struct customer {
	char *email;
	char *name;
	char *shoe_size;
	char *fav_food;
	struct customer *next;
} customer

```		
# hash

For the hash I just copied the same one that was used in the hashtable demo. Credit to Dan Bernstein and Alex for showing it.

# adding a customer

For adding a customer I referenced the hashtable from the demos but I would need to add a way to see if the email was already in the database. I would do this by comparing the inputed email to the emails that are already in the hash table. If they were already in the hash table I would just overwrite the data. If they weren't in the hash table I would just add it normally.

```
	
	This is a void function named add_customer that takes in a email char, name char, shoe size char, favorite food char, buckets, and the number of buckets
			initialize a bool to know whether or not we added them into the hash table. set this as false
			figure out which bucket we are in by taking the hash of the email and then modulo by the number of buckets
			create a new node 
			set the new node to be equal to buckets[which bucket]

			while the node is not NULL {
				if the email is already in the hashtable		//in this case I would use strcmp
						then set each value of the node equal to the email, name, shoe size, and favorite food
						set the bool to true
				
				go to the next node

			}
			if (bool != true) {
				we would then want to add them to the list by calling the function add_customer_to_list
			}
```

# add customer to list

For adding a customer to the list I also referenced the hashtable demo from a class. I would just need to add more arguments since there are more values

```
	create a new customer
	allocate the size for this new customer
	add each value to this new customer by doing strdup
	new_customer-> = strdup(email) doing this as well for name, shoe size and food
	new_customer->next = bucket;
	then return the new customer

```

# lookup

For lookup I would need to find which bucket the email is in. If I did find the email I would then print out that specified customers information. If the email was not found I would then prompt a message saying that the customer was not found. I also referenced the hashtable from the demos to do this. Specifcally the fav\_food\_for\_friend.

```
	This is a void function named lookup that takes in a email char, customer **buckets, and num\_buckets.

		figure out which bucket we are in by taking the hash of the email and then modulo by the number of buckets
		create a new customer node 
		set the node euqal to buckets[which bucket]
		initialize a bool to know whether or not we found an email. set to false
		
		while the node is not NULL {
			if the email is in the hash table
				print out each value 
				set the bool to true
		}
		
		if bool is not true {
			print out a error message saying we didn't find the customer
		}
```

# delete

For delete I would first want to check if the email is in the database before trying to delete it. I made a helper for this which return either true if the email is not in the database or false if the email is in the data base. [This image](https://imgur.com/a/rwvExwK) helped me visualed on how I should do my delete.

```
	This is a void function that takes in a email, customer **buckets, and number of buckets {
		find out which bucket we are in by taking the hash of the email and then modulo by the number of buckets
		set up a new node named node and set it equal to buckets[which bucket]
		make a new node named current and set it equal to this new node we just made

		if not found is false {
			if the email is the node {
				make the current = node
				make the node point to the next one. node -> next
				make the buckets[which buckets] = node
				free current
				then set current to null
			}else {
				make a new node named temp = node
				while temp is not null {
					if temp->next->email is the same as the email argument {
						make the current = to the next temp. temp->next
						then make the temp->next = temp->next->next so it connects the linked list back
						free current
					}
				temp = temp->next
				}
			}
		}
		else if not found is true {
			print out an error message saying we didn't find the customer
		}
	}
				
```
# not found

This is a helper function for delete in order to figure out whether or not the email is in the database. This is a bool function that either return true if the email is not in the database or false if the email is in the database.
```
	This is a bool function that takes in an email, customer **buckets and numbers of buckets {
		find out which bucket we are in by taking the hash of the email and then modulo by the number of buckets
		set up a new node named node and set it equal to buckets[which bucket]
		while node is not NULL {
			if node->email is the same as the email argument {
				return false
			}
			node = node->next
		}
		return true if we did find an email in the database
	}			
```
# list

For listing out the customer, I would want to loop through the buckets and then print each value from every single customer.

```
	This is a void function that takes in customer **buckets and num_buckets as arguments {
		loop through the num buckets with i as the counter
			create a new node and set it equal to buckets[i]
			while node is not null
				print out each value
				point to the next one by node->next
			}
		}
	
	}
```

# loading the customer database from a file

For reading the file I would want to use fopen then use fgets for reading. After that I would then use sscanf to read formatted input which is from the file that is filled with char that are tab separated. I also referenced this from hw5 doing load vocabulary.

```
	This is a void function that takes in a filename, customer **buckets, and num_buckets {
		initialize email, name, shoe size, and favorite food as a char with the size of 256
		initialize a buffer with a size of 256 in this case
		open the file using fopen. In this case I named it as infile.
		
		while fgets (buf, BUFSIZE = 256, infile)) {
			using sscanf to read in and the format would be %[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n. sscanf(buf, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n", email, name, shoe size, fav food)
			after reading these in I would then add them as a customer using those same values (email, name, shoe size, favorite food) and buckets and num_buckets
		}
	close the file so we don't have any memory leaks
	}
```

# saving the customer data base to a file

For writing to the file I would need to use fopen write. After that I would loop through the num buckets and dump each value from each customer in a format where it splits them by tabs. I would use fprintf to put each of the customer data in.

```
	This is a void function that takes in a filename, customer **bucekts, and num_buckets {
		Open up the file using fopen for writing
		loop through the num buckets with i as the counter {
		set a new customer node = buckets[i]
		while the node is not NULL {
			use fprintf with the format %s\t%s\t%s\t%s\n to print each value in from the new customer node
			then goes to the next node. node->next
			}
		}
	}
```

# freeing

For freeing all the customer values I would want to loop through the number of buckets and then free each customer value. I would also need to free the current node.

```
	This is a void function that takes in the customer **buckets and the number of buckets {
		loop through the number of buckets with i as the counter {
			make a new node called current setting it equal to buckets[i]
			while current is not null { 
				free each value by doing free(current->email) doing this as well for name, shoe sizze and favorite food
				new a new temp node that is equal to the current node
				free this temp value
				then point to the next current. current -> next
			}
		}
	}
```
	
# reading in user commands / the main part

For this part of the program I will plan to use fgets or scanf for the user input and then after use strncmp cases to see what commands they put in

```
	set the customer *buckets[NUM_BUCKETS] = NULL		//NUM BUCKETS was defined as 100 for this whole program
	load in the customer database from the tsv file by calling the function load

	while (1) {	//I set the while loop as 1 until we want to exit out and stop giving out prompts
		prompt the user to type a command
		use fgets to get the command they put in

		using strncmp to see if they typed in "exit" {
			call the free function
			print a prompt saying we are exiting and freeing memory
			break out of the while loop
		}
		using strncmp to see if they typed in "add" {
			initialize a char email, char name, char shoe size, and char favorite food with a size of 256
			prompt the user to type in a email
			use fgets to get the email they put in
			prompt the user to type in a name
                        use fgets to get the name they put in
			prompt the user to type in a shoe size
                        use fgets to get the shoe size they put in
			prompt the user to type in a favorite food
                        use fgets to get the favorite food they put in
			since fgets decides to add a new line at the end I had to remove it from each one by doing email[strlen(email) -1] = 0. I did this for each value.
			call the function add customer by add_customer(email, name, shoe_size, fav_food, buckets, NUM_BUCKETS);
		}
		using strncmp to see they typed in "lookup" {
			initalize a char email with a size of 256
			prompt the user to type in an email
			use fgets to get the email they typed in
			remove the new line from fgets doing the same as above
			call the function lookup by loopup(email, buckets, NUM_BUCKETS)
		}
		using strncmp to see if they typed in "list" {
			calls the function list with arguments buckets and NUM_BUCKETS
		}
		using strncmp to see if they typed in "delete" {
			initialize a char email with a size of 256
			prompt the user to type in an email
			use fgets to get the email they type in
			remove the new line from using fgets doing the same from add
			call the function delete by using the email, buckets and NUM_BUCKETS as arguments
		}
		using strncmp to see if they typed in "save" {
			call the function save by using customers.tsv, buckets and NUM_BUCKETS as arguments
		}
	}
						
```


