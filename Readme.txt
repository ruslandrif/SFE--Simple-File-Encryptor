On the fifth commit the encryption algorithm was improved.
Now, it use the threadsafe queue (with condition variables and mutexes) to 
allow parallel reading and processing information from files.
Also final screen was added with the encryption time.