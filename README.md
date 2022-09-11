# Server-Client-Disease-Aggregator
    Folders

        Parent Folder: Server-Client-Disease-Aggregator
                SubFolders:
                        Master
                                Source folder     : Master/src
                                Header folder     : Master/include
                                Build folder      : Mmaster/build        
                                Etc folder        : Master/etc            

                        WhoClient
                                Source folder     : WhoClient/src
                                Header folder     : WhoClient/include
                                Build folder      : WhoClient/build        
                                Etc folder        : WhoClient/etc          

                        WhoServer

                                Source folder     : WhoServer/src
                                Header folder     : WhoServer/include
                                Build folder      : WhoServer/build         




    Compilation & Execution

        Compile         : make rebuild
        Execution       : You have to open three different terminals and execute the following:
                                1)  ./whoServer -q 5000 -s 6000 -w 15 -b 50
                                2)  ./master -w 5 -b 50 -s linux01.di.uoa.gr -p 6000 -i Master/etc/input_dir                (if you want to run it locally , replace linux01.di.uoa.gr with localhost)
                                3)  ./whoClient -q WhoClient/etc/queryFile.txt -w 10 -sp 5000 -sip linux01.di.uoa.gr        (same)

     
    Extra Tools

        Valgrind        : For memory leaks and errors
        GDB             : For bugs

