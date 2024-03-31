#include <stdio.h> // printf()
#include <unistd.h> // getopt()
#include <getopt.h> // getopt_long()
#include <stdlib.h> // exit(), getenv()
#include <stdbool.h> // bool

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

void help(char* executable_path) {
	char* home_directory_path = getenv("HOME");
	if (home_directory_path == NULL) {
		fprintf(stderr, "Unable to read home directory path from 'HOME' environment variable!\n");
		exit(EXIT_FAILURE);
	}

	printf("Virtual Machine Manager (v%d.%d.%d)\n", VMM_MAJOR_VERSION, VMM_MINOR_VERSION, VMM_PATCH_VERSION);
	printf(" A utility for orchestrating QEMU virtual machines, inspired by Docker.\n\n");

	printf("Usage: %s [options] [action] [machine,...]\n\n", executable_path);

	printf("Options:\n");
		printf("\t-?, -h, --help / VMM_HELP=true\n");
			printf("\t\tShow this help message & exit.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-V, --version / VMM_VERSION=true\n");
			printf("\t\tShow the semantic version & exit.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-v, --verbose / VMM_VERBOSE=true\n");
			printf("\t\tIncrease output verbosity to aid debugging.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-q, --quiet / VMM_QUIET=true\n");
			printf("\t\tHide all output excluding errors while running.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-e, --extended / VMM_EXTENDED=true\n");
			printf("\t\tShow additional columns when listing virtual machines.\n");
			printf("\t\tOnly applicable to the 'list' action.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-n, --no-confirm / VMM_CONFIRM=false\n");
			printf("\t\tBypass confirmation when performing destructive actions (e.g., destroying a virtual machine).\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-d, --include-drives / VMM_DRIVES=true\n");
			printf("\t\tInclude persistent data drives when destroying, backing up & restoring.\n");
			printf("\t\tOnly applicable to the 'destroy', 'backup' & 'restore' actions.\n");
			printf("\t\tDefault: false.\n\n");

		printf("\t-c[=]<path>, --config[=]<path> / VMM_CONFIG=<path>\n");
			printf("\t\tSet the path to the global configuration file\n");
			printf("\t\tDefault: '/etc/vmm/global.yaml' (system), '%s/.config/vmm/global.yaml' (user).\n\n", home_directory_path);

		printf("\t-g, --generate-config / VMM_GENERATE_CONFIG=true\n");
			printf("\t\tShow an example global configuration file & exit.\n");
			printf("\t\tDefault: false.\n\n");

	printf("Actions:\n");
		printf("\tls, list / VMM_ACTION=list\n");
			printf("\t\tShow the status of all virtual machine(s).\n");
			printf("\t\tSee the -e/--extended option.\n\n");

		printf("\tinit, initialise / VMM_ACTION=initialise\n");
			printf("\t\tCreate new virtual machine(s).\n\n");

		printf("\tcreate / VMM_ACTION=create\n");
			printf("\t\tLaunch the specified virtual machine(s) without starting the guest operating system.\n");
			printf("\t\tThis puts the specified virtual machine(s) into an idle state.\n\n");

		printf("\tstart / VMM_ACTION=start\n");
			printf("\t\tStart the guest operating system on the specified virtual machine(s).\n\n");

		printf("\tstop / VMM_ACTION=shutdown\n");
			printf("\t\tForcefully stop the specified virtual machine(s).\n\n");

		printf("\tshutdown / VMM_ACTION=shutdown\n");
			printf("\t\tGracefully (ACPI) shutdown the guest operating system on the specified virtual machine(s).\n\n");

		printf("\tpause, freeze / VMM_ACTION=pause\n");
			printf("\t\tFreeze execution of the specified virtual machine(s).\n\n");

		printf("\tresume, thaw / VMM_ACTION=resume\n");
			printf("\t\tUnfreeze execution of the specified virtual machine(s).\n\n");

		printf("\tremove / VMM_ACTION=remove\n");
			printf("\t\tRemove the specified virtual machine(s) (does not delete files).\n");
			printf("\t\tThis puts the specified virtual machine(s) into a dormant state.\n\n");

		printf("\tdestroy, nuke / VMM_ACTION=destroy\n");
			printf("\t\tDeletes the specified virtual machine(s).\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives option.\n\n");

		printf("\tbackup, archive / VMM_ACTION=backup\n");
			printf("\t\tBackup the specified virtual machine(s).\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives option.\n\n");

		printf("\trestore / VMM_ACTION=restore\n");
			printf("\t\tRestore the specified virtual machine(s) from a backup.\n\n");
			printf("\t\tThis includes configuration files & data directories!\n");
			printf("\t\tSee the -d/--drives option.\n\n");

		printf("\tThis positional argument can be omitted if the 'VMM_ACTION' environment variable is set.\n\n");

	printf("The 'machine' positional argument is a comma-separated list of virtual machine names and/or unique identifiers to perform the action on.\n");
	printf(" This can be specified via the 'VMM_MACHINE=machine,...' environment variable too, in which case the positional argument can be omitted.\n");
	printf(" This defaults to '*' for the 'list' action to target all virtual machines, but is required for any other actions.\n");
	printf(" The 'create' action does not accept unique identifiers and as such will treat them as names.\n\n");

	printf("Command-line options & positional arguments take precedence over their corresponding environment variables.\n");
	printf(" Boolean environment variables (e.g. / VMM_VERBOSE) accept '1' or 'true' as positive, and '0' or 'false' as negative.\n\n");

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
		printf("\tQEMU for running virtual machines & generating persistent drives (https://qemu.org).\n");
		printf("\tKVM for hardware acceleration (https://linux-kvm.org).\n");
		printf("\tSWTPM for emulating trusted platform modules (https://github.com/stefanberger/swtpm).\n");
		printf("\tEDK II OVMF for enabling UEFI-compatible systems (https://github.com/tianocore/tianocore.github.io/wiki/OVMF).\n");
		printf("\tOpenSSL for generating crytographic secrets (https://openssl.org).\n");
		printf("\tGnuPG for PGP email encryption & signing (https://gnupg.org).\n\n");

	printf("Crafted with \u2764\uFE0F  by viral32111 (contact@viral32111.com; https://viral32111.com).\n");
	printf(" Contribute freely on GitHub at https://github.com/viral32111/virtual-machine-manager.\n");
	printf(" Proudly open-source under the GNU AGPL v3.0 (https://gnu.org/licenses/agpl-3.0.html).\n");
}

int main(int argument_count, char* argument_values[]) {
	if (argument_count <= 0) {
		fprintf(stderr, "No arguments passed!\n");
		return EXIT_FAILURE;
	}

	// Prevents getopt_long() from printing its own error messages to the standard error output
	opterr = 0;

	static struct option long_options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "verbose", no_argument, NULL, 'v' },
		{ NULL, 0, NULL, 0 }
	};

	bool be_verbose = false;

	while (1) {
		int option_index = 0;

		// The index is incremented for us
		int option_value = getopt_long(argument_count, argument_values, "hv", long_options, &option_index);
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
				be_verbose = true;
				break;

			// Option is not in the long_options array
			case '?':
				fprintf(stderr, "Unrecognised option '%c'!\n", optopt);
				exit(EXIT_FAILURE);
				break;

			// This should never happen
			default:
				fprintf(stderr, "Impossible scenario for option '%c'!\n", option_value);
				exit(EXIT_FAILURE);
				break;
		}
	};

	if (optind < argument_count) {
		printf("Non-option arguments: ");
		while (optind < argument_count) printf("%s ", argument_values[optind++]);
		printf("\n");
	}

	if (be_verbose) printf("Exiting with success.\n");
	return EXIT_SUCCESS;
}
