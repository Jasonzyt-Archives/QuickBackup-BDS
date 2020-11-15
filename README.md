# QuickBackup-BDS
A QuickBackup & QuickBack Plugin Of Bedrock Dedicated Server 

#### Thanks
+ Thanks for the inspiration support of [TISUnion](https://www.github.com/TISUnion/QuickBackupM)     
+ Thanks for the code support of [BDS-JSRunner](https://mzgl.coding.net/public/BDSJSRunner/BDSJSRunner/git/files/master/BDSJSRunner)     
#### Files
+ QuickBackup/Backup.json     
  Backup records json    
+ QuickBackup/config.ini    
  Config file    
+ QuickBackup/From.json    
  Backup records json    
+ QuickBackup/qb.log    
  Backup log file    
+ QuickBackup/bz/*    
  Bandzip console applications    
#### Plugin Commands
##### Server Commands
+ qb_backup   
  Run once backup (NOT AutoBackup)
+ qb_reload   
  Reload config file
+ qb_fix  
  Run fix function:  
  Downlaod config file and bandzip console application
  + [config.ini](http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini)  
  + [bz.exe](http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe) 
  + [ark.x64.dll](http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll) 
  + [ark.x86.dll](http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll) 
  + [ark.x64.lgpl.dll](http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll) 
  + [ark.x86.lgpl.dll](http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll) 

##### Client Commands
+ /qb_backup  
  Run once backup (NOT AutoBackup)
+ /qb_reload  
  Reload config file