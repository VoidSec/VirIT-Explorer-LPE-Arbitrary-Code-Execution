# VirIT-Explorer-LPE-Arbitrary-Code-Execution
A proof of concept for VirIT Explorer (Lite & Pro) <=v.8.1.68 Local Privilege Escalation (System)/Arbitrary Code Execution 

## How to Use

Add Admin:

    -In ServiceMain comment the line 91: Spawn_Shell(); and de-comment the line 92: Add_Admin();
    -decomment the while line 95-97
    -change Add_Admin() function line 59 replacing VoidSec with your chosen username and replace secret with your chosen password
    -compile the file as viritsvc.exe
    -replace the viritsvc.exe located at C:\VEXPLite on the target machine
    -reboot
    -it will spawn a new admin account on login screen
    
![Preview](https://voidsec.com/wp-content/uploads/2016/05/adduser.png)
    
Spawn Reverse Shell:
  
    -generate a Meterpreter reverse payload (windows/meterpreter/reverse_tcp) and replace the content of the var r_shell[]
    -start a Meterpreter handler to catch the reverse shell
    -compile the file as viritsvc.exe
    -replace the viritsvc.exe located at C:\VEXPLite on the target machine
    -reboot
    -it will spawn a reverse shell in ~5 minutes
    
![Preview](https://voidsec.com/wp-content/uploads/2016/05/system.png)

### Vulnerability

I've made this [blog post](https://voidsec.com/the-curse-of-the-antivirus-solution/) explaining the vulnerability.
Video Poc [here](https://www.youtube.com/watch?v=5a09efEvjTk)
