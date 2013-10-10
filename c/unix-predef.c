//main(){printf("%s\n", "fun"-0x60);}
main(){
printf("1%s\n", "\021\012"); //oct 17 10
printf("2%s\n", "\051\052"); //oct 41 42
printf("3%c\n", (1)["have"]);
//printf("%s\n", (1)["have"]+"fun");
printf("4%s\n", (1)["have"]+"fun"-0x60);
printf(&unix["\021%six\012\0"],(unix)["have"]+"fun"-0x60);
}
