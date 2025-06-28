clang -S -emit-llvm vuln.c -o vuln.llcd 

opt -load ./libSecurityPass.so -securitypass ../vuln.ll -o /dev/null