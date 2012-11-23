About logger
---------------------------------------------------------------------
This is c++ library use for write log to file


Support
---------------------------------------------------------------------
1. This library can work linux/windows.
2. Logger is multi-thread safe.
3. On Linux is multi-process safe.
4. On Linux support change log file owner when you need.
5. Support log file auto split by to way and cycle log to you point directory:

1) split log file by every day,after split the logfile name like 'name'.year-mon-day

2) split log file by log max size,at you can point at Init(),after split logfile the name like 'name'.number,the number in [1-5]

Contact
---------------------------------------------------------------------
If you have some problem of logger,you can send email to <imawljiang@gmail.com> <imawljiang at gmail.com>