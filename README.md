# pl4gu3
PoC of some functionality of the plague PAM-based backdoor discovered and documented in 2025 by Nextron Systems 

## Disclaimer

The code in this repository is purely for educational purposes, I'm not responsible for any missuse of it in any shape or form.

## Setting up

To use the malware, it requires the package `libpam0g-dev` to be compiled, any custom PAM module can be compiled with the commands:

```bash
    gcc -fPIC -fno-stack-protector -c module_code.c -o module_object.o
    gcc -shared -o module.so module_object.o -lpam
```

This `.so` file is the binary to be injected.

PAM modules tipically reside in `/lib/x86_gnu-linux/security/` or any similar directory, depending on the machine.

After putting the malware in its place, we can modify any PAM config file used for auth mode, but the most use is found in `/etc/pam.d/common-auth`, since it is the most imported and basic form of authentication for any login or app that uses login (for example, OpenSSH and the sshd daemon). For plague to work, we have to add the following line to the top of this config file:

```C
auth   sufficient   module.so
```

Whenever common-auth is called, it will check if the password given is equal to the "magic word" we have established, and if so, it's gonna skip the rest of the required steps and checks and let us through, authenticaded as any user requested.

## Techniques used 

### Magic Password

Perhaps the main functionality of plague is the backdoor itself, often a magic password to access any user through any login process that uses the PAM authentication method. This comes in the form of a custom PAM module invoked with the mode "sufficient" in the common-auth PAM config file, which is imported for most login processes in modern GNU/Linux systems.

This method implements a hard-coded string (or any other type, useful in static ofbuscation) used as the "magic word" to input as password for any user in the system to bypass its regular login.

### Malicius session

