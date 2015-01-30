Troubleshooting
===============

Network configuration

    # show wlan interfaces
    netsh wlan show interface
    # show ip configuration
    ipconfig


Restart olsrd manually

    # kill olsrd
    Taskkill /IM olsrd.exe /F
    # start olsrd
    ## interface number is hex encoded
    .\olsrd.exe -i if24 -f .\olsrd_app.conf -d 2
	

