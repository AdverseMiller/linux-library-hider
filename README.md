# linux-library-hider
A very simple driver that uses kprobes to hide library files (or any mapped file) from the maps of a process.

Before (normal):
![Before (normal)](before.png)

After (hidden):
![After (hidden)](after.png)