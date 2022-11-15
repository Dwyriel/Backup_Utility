# Backup_Utility

A backup utility made with Qt

## How to Use

1 - Select a backup Folder. It's going to be used for all backups while keeping things organized inside.<br />
2 - Create a preset. Presets will help you organized what files you want to backup when you click 'Backup!'.<br />
3 - Select the files and folder you want to backup for the selected preset.<br />
4 - Click Backup!<br />
5 - Posterior usages are as simples as opening the program and clicking Backup!<br />

Backup Folder will look something like this:

```bash
BackupFolder
├── Preset1
│   ├── Backup 1
│   │   ├── Folder1
│   │   │   ├── SubFolder1
│   │   │   │   └── FileInsideSubfolder1.file
│   │   │   └── SubFolder2
│   │   │       └── FileInsideSubfolder2.file
│   │   ├── Folder2
│   │   │   └── FileInsideFolder2.file
│   │   ├── File1.file
│   │   ├── File2.file
│   │   └── File3.file
│   ├── Backup 2
│   │   └── More Files and Folders
│   └── More Backups
├── Preset2
│   └── Backup 1
│       └── Files and Folders
└── Etc..
```
