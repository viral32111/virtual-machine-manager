#include <stdio.h> // printf()
#include <unistd.h> // getopt()
#include <getopt.h> // getopt_long()
#include <stdlib.h> // exit(), getenv()
#include <stdbool.h> // bool
#include <string.h> // strcat()

// Sanity checks
#ifndef VMM_MAJOR_VERSION
#error "Compiler macro 'VMM_MAJOR_VERSION' is not defined!"
#endif
#ifndef VMM_MINOR_VERSION
#error "Compiler macro 'VMM_MINOR_VERSION' is not defined!"
#endif
#ifndef VMM_PATCH_VERSION
#error "Compiler macro 'VMM_PATCH_VERSION' is not defined!"
#endif

#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

bool is_root() {
	return geteuid() == 0;
}

char* get_home_directory() {
	char* home_directory_path = getenv("HOME");
	if (home_directory_path == NULL) {
		fprintf(stderr, "Unable to read home directory path from 'HOME' environment variable!\n");
		exit(EXIT_FAILURE);
	}

	return home_directory_path;
}

void help(char* executable_path) {
	char* home_directory_path = get_home_directory();

	printf("Virtual Machine Manager v%d.%d.%d (%s, %s)\n", VMM_MAJOR_VERSION, VMM_MINOR_VERSION, VMM_PATCH_VERSION, __DATE__, __TIME__);
	printf(" A utility for orchestrating QEMU virtual machines, inspired by Docker.\n\n");

	DEBUG_PRINT("You're using a debug build! Hello from '%s:%d'.\n\n", __FILE__, __LINE__);

	printf("Usage: %s [flags] [action] [machine,...]\n\n", executable_path);

	printf("Flags:\n");
		printf("\t-h, --help, VMM_HELP=true\n");
			printf("\t\tShow this help message & exit.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-V, --version, VMM_VERSION=true\n");
			printf("\t\tShow the semantic version & exit.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-v, --verbose, VMM_VERBOSE=true\n");
			printf("\t\tIncrease output verbosity to aid debugging.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-q, --quiet, VMM_QUIET=true\n");
			printf("\t\tHide all output excluding errors while running.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-e, --extended, VMM_EXTENDED=true\n");
			printf("\t\tShow additional columns when listing virtual machines.\n");
			printf("\t\tOnly applicable to the 'list' action.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-n, --no-confirm, VMM_CONFIRM=false\n");
			printf("\t\tBypass confirmation when performing destructive actions (e.g., destroying a virtual machine).\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-d, --include-drives, VMM_DRIVES=true\n");
			printf("\t\tInclude persistent data drives when destroying, backing up & restoring.\n");
			printf("\t\tOnly applicable to the 'destroy', 'backup' & 'restore' actions.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-c[=]<path>, --config[=]<path>, VMM_CONFIG=<path>\n");
			printf("\t\tSet the path to the global configuration file\n");
			printf("\t\tDefault: '/etc/vmm/global.yaml' (system), '%s/.config/vmm/global.yaml' (user).\n\n", home_directory_path);

		printf("\t-g, --generate-config, VMM_GENERATE_CONFIG=true\n");
			printf("\t\tShow an example global configuration file & exit.\n");
			printf("\t\tDefault: false.\n\n");

	printf("Actions:\n");
		printf("\tls, list, VMM_ACTION=list\n");
			printf("\t\tShow the status of all virtual machine(s).\n");
			printf("\t\tSee the -e/--extended flag.\n\n");

		printf("\tinit, initialise, initialize, VMM_ACTION=initialise\n");
			printf("\t\tCreate new virtual machine(s).\n\n");

		printf("\tcreate, VMM_ACTION=create\n");
			printf("\t\tLaunch the specified virtual machine(s) without starting the guest operating system.\n");
			printf("\t\tThis puts the specified virtual machine(s) into an idle state.\n\n");

		printf("\tstart, VMM_ACTION=start\n");
			printf("\t\tStart the guest operating system on the specified virtual machine(s).\n\n");

		printf("\tstop, VMM_ACTION=shutdown\n");
			printf("\t\tForcefully stop the specified virtual machine(s).\n\n");

		printf("\tshutdown, VMM_ACTION=shutdown\n");
			printf("\t\tGracefully (ACPI) shutdown the guest operating system on the specified virtual machine(s).\n\n");

		printf("\tpause, freeze, VMM_ACTION=pause\n");
			printf("\t\tFreeze execution of the specified virtual machine(s).\n\n");

		printf("\tresume, thaw, VMM_ACTION=resume\n");
			printf("\t\tUnfreeze execution of the specified virtual machine(s).\n\n");

		printf("\tremove, VMM_ACTION=remove\n");
			printf("\t\tRemove the specified virtual machine(s) (does not delete files).\n");
			printf("\t\tThis puts the specified virtual machine(s) into a dormant state.\n\n");

		printf("\tdestroy, nuke, VMM_ACTION=destroy\n");
			printf("\t\tDeletes the specified virtual machine(s).\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives flag.\n\n");

		printf("\tbackup, archive, VMM_ACTION=backup\n");
			printf("\t\tBackup the specified virtual machine(s).\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives flag.\n\n");

		printf("\trestore, VMM_ACTION=restore\n");
			printf("\t\tRestore the specified virtual machine(s) from a backup.\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives flag.\n\n");

		printf("\tThis positional argument can be omitted if the 'VMM_ACTION' environment variable is set.\n\n");

	printf("The 'machine' positional argument is a comma-separated list of virtual machine names and/or unique identifiers to perform the action on.\n");
	printf(" This can be specified via the 'VMM_MACHINE=machine,...' environment variable too, in which case the positional argument can be omitted.\n");
	printf(" This defaults to '*' for the 'list' action to target all virtual machines, but is required for all other actions.\n");
	printf(" The 'create' action does not accept unique identifiers and as such will treat them as names.\n\n");

	printf("Command-line flags & positional arguments take precedence over their corresponding environment variables.\n");
	printf(" Boolean environment variables (e.g. VMM_VERBOSE) accept '1', 'true', or 'yes' as positive, and '0', 'false', or 'no' as negative. Other values will cause an error.\n\n");

	printf("Examples:\n");
		printf("\t%s initialise demo\n", executable_path);
			printf("\t\tInitialise a new virtual machine with the name 'demo'.\n\n");

		printf("\t%s -c /etc/vmm.yaml --extended list\n", executable_path);
			printf("\t\tUses the custom configuration file at '/etc/vmm.yaml' & lists all virtual machines with additional columns.\n\n");

		printf("\tVMM_VERBOSE=true %s --no-confirm destroy demo\n", executable_path);
			printf("\t\tDeletes the virtual machine with the name 'demo' without asking for confirmation & outputs additional information while doing so.\n\n");

		printf("\tVMM_ACTION=backup VMM_DRIVES=true VMM_MACHINE=00000000-0000-0000-0000-000000000000 %s\n", executable_path);
			printf("\t\tBacks up the configuration files & data directories for the virtual machine with the unique identifier '00000000-0000-0000-0000-000000000000', including its persistent data drives.\n\n");

	printf("Dependencies:\n");
		printf(" QEMU for running virtual machines & generating persistent drives (https://qemu.org).\n");
		printf(" KVM for hardware acceleration (https://linux-kvm.org).\n");
		printf(" SWTPM for emulating trusted platform modules (https://github.com/stefanberger/swtpm).\n");
		printf(" EDK II OVMF for enabling UEFI-compatible systems (https://github.com/tianocore/tianocore.github.io/wiki/OVMF).\n");
		printf(" OpenSSL for generating cryptographic secrets (https://openssl.org).\n");
		printf(" GnuPG for PGP email encryption & signing (https://gnupg.org).\n\n");

	printf("Crafted with \u2764\uFE0F by viral32111 (contact@viral32111.com; https://viral32111.com).\n");
	printf(" Contribute freely on GitHub at https://github.com/viral32111/virtual-machine-manager.\n");
	printf(" Proudly open-source under the GNU AGPL v3.0 (https://gnu.org/licenses/agpl-3.0.html).\n");
}

int main(int argument_count, char* argument_values[]) {
	// This should never happen, as the first argument should always be the binary path
	if (argument_count <= 0) {
		fprintf(stderr, "No arguments passed!\n");
		return EXIT_FAILURE;
	}

	// Prevents getopt_long() from printing its own error messages to the standard error output
	opterr = 0;

	static struct option long_options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ "verbose", no_argument, NULL, 'v' },
		{ "quiet", no_argument, NULL, 'q' },
		{ "extended", no_argument, NULL, 'e' },
		{ "no-confirm", no_argument, NULL, 'n' },
		{ "include-drives", no_argument, NULL, 'd' },
		{ "config", optional_argument, NULL, 'c' },
		{ "generate-config", no_argument, NULL, 'g' },
		{ NULL, 0, NULL, 0 } // Required to terminate the array
	};

	short flag_verbosity_level = 1; // 0 = quiet, 1 = default, 2 = verbose
	bool flag_show_additional_columns = false;
	bool flag_bypass_confirmation = false;
	bool flag_include_drives = false;
	char* flag_configuration_file_path = NULL;

	// Default for the configuration file path
	if (is_root()) {
		printf("We have super awesome powers!\n\n");
		flag_configuration_file_path = "/etc/vmm/global.yaml";
	} else {
		// TODO: Consider XDG_DATA_HOME, XDG_CONFIG_HOME, XDG_DATA_DIRS, XDG_CONFIG_DIRS, XDG_CACHE_HOME, XDG_RUNTIME_DIR, etc.
		char* home_directory_path = get_home_directory();

		flag_configuration_file_path = home_directory_path;
		strcat(flag_configuration_file_path, "/.config/vmm/global.yaml");
	}

	while (1) {
		int option_index = 0;

		// The index is incremented for us
		int option_value = getopt_long(argument_count, argument_values, "hVvqendcg", long_options, &option_index);
		if (option_value == -1) break;

		switch (option_value) {
			// -h / --help
			case 'h':
				help(argument_values[0]);
				exit(EXIT_SUCCESS);
				break;

			// -V / --version
			case 'V':
				printf("%d.%d.%d\n", VMM_MAJOR_VERSION, VMM_MINOR_VERSION, VMM_PATCH_VERSION);
				exit(EXIT_SUCCESS);
				break;

			// -v / --verbose
			case 'v':
				printf("Henceforth, we shall be verbose.\n");
				flag_verbosity_level = 2;
				break;

			// -q / --quiet
			case 'q':
				flag_verbosity_level = 0;
				break;

			// -e / --extended
			case 'e':
				if (flag_verbosity_level >= 2) printf("Additional columns will be shown.\n");
				flag_show_additional_columns = true;
				break;

			// -n / --no-confirm
			case 'n':
				if (flag_verbosity_level >= 2) printf("All confirmation prompts will be skipped.\n");
				flag_bypass_confirmation = true;
				break;

			// -d / --include-drives
			case 'd':
				if (flag_verbosity_level >= 2) printf("Persistent data drives will be considered in actions.\n");
				flag_include_drives = true;
				break;

			// -c / --config
			case 'c':
				if (optarg == NULL) {
					fprintf(stderr, "No value given for configuration file path flag!\n");
					exit(EXIT_FAILURE);
				}
				flag_configuration_file_path = optarg;
				break;

			// Option is not in the long_options array
			case '?':
				fprintf(stderr, "Unrecognised flag '-%c'!\n", optopt);
				exit(EXIT_FAILURE);
				break;

			// This should never happen
			default:
				fprintf(stderr, "Impossible scenario for option '%c'!\n", option_value);
				exit(EXIT_FAILURE);
				break;
		}
	};

	if (flag_verbosity_level >= 1) {
		printf("Verbosity level: %d\n", flag_verbosity_level);
		printf("Show additional columns: %s\n", flag_show_additional_columns ? "true" : "false");
		printf("Bypass confirmation prompts: %s\n", flag_bypass_confirmation ? "true" : "false");
		printf("Include persistent drives: %s\n", flag_include_drives ? "true" : "false");
		printf("Configuration file path: '%s'\n\n", flag_configuration_file_path);
	}

	if (optind >= argument_count) {
		printf("You want a list all virtual machines!\n");
	} else {
		while (optind < argument_count) {
			char* argument = argument_values[optind++];

			if (strcmp(argument, "ls") == 0 || strcmp(argument, "list") == 0) {
				printf("You want a list all virtual machines!\n");
			} else if (strcmp(argument, "init") == 0 || strcmp(argument, "initialise") == 0 || strcmp(argument, "initialize") == 0) {
				printf("You want to create virtual machine(s)!\n");
			} else if (strcmp(argument, "create") == 0) {
				printf("You want to launch virtual machine(s)!\n");
			} else if (strcmp(argument, "start") == 0) {
				printf("You want to start virtual machine(s)!\n");
			} else if (strcmp(argument, "stop") == 0) {
				printf("You want to stop virtual machine(s)!\n");
			} else if (strcmp(argument, "shutdown") == 0) {
				printf("You want to shutdown virtual machine(s)!\n");
			} else if (strcmp(argument, "pause") == 0 || strcmp(argument, "freeze") == 0) {
				printf("You want to pause virtual machine(s)!\n");
			} else if (strcmp(argument, "resume") == 0 || strcmp(argument, "thaw") == 0) {
				printf("You want to resume virtual machine(s)!\n");
			} else if (strcmp(argument, "remove") == 0) {
				printf("You want to remove virtual machine(s)!\n");
			} else if (strcmp(argument, "destroy") == 0 || strcmp(argument, "nuke") == 0) {
				printf("You want to destroy virtual machine(s)!\n");
			} else if (strcmp(argument, "backup") == 0 || strcmp(argument, "archive") == 0) {
				printf("You want to backup virtual machine(s)!\n");
			} else if (strcmp(argument, "restore") == 0) {
				printf("You want to restore virtual machine(s)!\n");
			} else {
				fprintf(stderr, "Unrecognised action '%s'!\n", argument);
				fprintf(stderr, "Run '%s --help' for more information.\n", argument_values[0]);
				exit(EXIT_FAILURE);
			}
		}
	}

	if (flag_verbosity_level >= 2) printf("Exiting with success.\n");
	return EXIT_SUCCESS;
}
