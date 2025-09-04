cargo build --release

copy %~dp0target\release\server-rust.exe ..\bin\
copy %~dp0target\release\client-rust.exe ..\bin\
pause