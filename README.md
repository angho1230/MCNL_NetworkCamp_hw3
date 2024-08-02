# MCNL_NetworkCamp_hw3

# Remote ssh!!
> Do your work remotely with my app!!d

## How to Make

You will have two `Makefiles` in `client` and `server` directory so to make it individually
>make

## About My Project 
The project contains several components:
1. Downloading selected files from the server.
2. Uploading selected files to the server.
3. Change your directory with cd command

## How to Execute
1. executables in server side : `server` `<port>`
>./server 10000
2. executables in client side : `client` `<Server IP>` `<port>` 
>./client 127.0.0.1 10000
3. use `cd` `<path>` to change your directory
4. use `ls` to list the files in your current directory
5. use `up` or `dl` to upload or download the files
>up <file_name>
>dl <file_name>
