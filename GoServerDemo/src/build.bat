REM Static Compile
go build -o ../server.exe -ldflags "-s -w" -tags netgo -installsuffix netgo server.go