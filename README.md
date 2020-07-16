# Server-Client-Disease-Aggregator
    Folders

        Parent Folder: Server-Client-Disease-Aggregator
                SubFolders:
                        Master
                                Source folder     : Master/src
                                Header folder     : Master/include
                                Build folder      : Mmaster/build         (This folder holds .o files)
                                Etc folder        : Master/etc            (This fodler holds the input files, logfiles, fifo files)

                        WhoClient
                                Source folder     : WhoClient/src
                                Header folder     : WhoClient/include
                                Build folder      : WhoClient/build         (This folder holds .o files)
                                Etc folder        : WhoClient/etc           (This folder holds queryFile.txt)

                        WhoServer

                                Source folder     : WhoServer/src
                                Header folder     : WhoServer/include
                                Build folder      : WhoServer/build         (This folder holds .o files)




    Compilation & Execution

        Compile         : In general folder open a terminal and execute this command:          make rebuild
        Execution       : You have to open three different terminals and execute the following commands with EXACTLY same order:
                                1)  ./whoServer -q 5000 -s 6000 -w 15 -b 50
                                2)  ./master -w 5 -b 50 -s linux01.di.uoa.gr -p 6000 -i Master/etc/input_dir                (if you want to run it locally , replace linux01.di.uoa.gr with localhost)
                                3)  ./whoClient -q WhoClient/etc/queryFile.txt -w 10 -sp 5000 -sip linux01.di.uoa.gr        (same comment as above)

    Explanation

       Master folder is a customized version of Disease-Aggregator repository. In this project, I extended Disease-Aggregator project by adding a server-client. In essence, I created a server which is going to communicate with a client and workers from master.
        Client's resbonsibility is to read queriesFile and for each query to create a thread that is going to send this query to server. After that server will forward those queries to workers while waiting for workers' results.


    Extra Tools

        Valgrind        : For memory leaks and errors
        GDB             : For bugs

