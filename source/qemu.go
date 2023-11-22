package main

import "fmt"
import "os"
import "os/exec"
import "path"

// qemu-img create -f qcow2 -o preallocation=full NAME.qcow2 64G
func createQEMUImage( fileName string, directoryPath string, sizeInMiB int, format string, shouldPreAllocate bool ) {
	var preAllocationOption string = "preallocation=off"
	if shouldPreAllocate {
		preAllocationOption = "preallocation=full"
	}

	var fileNameWithExtension string = fmt.Sprintf( "%s.%s", fileName, format )
	var filePath string = path.Join( directoryPath, fileNameWithExtension )

	var sizeWithSuffix string = fmt.Sprintf( "%dM", sizeInMiB )

	var command = exec.Command( "qemu-img", "create", "-f", format, "-o", preAllocationOption, filePath, sizeWithSuffix )
	var output, error = command.CombinedOutput()
	if error != nil {
		fmt.Fprintf( os.Stderr, "An error occured while attempting to create the image! (%s)\n", error.Error() )
		os.Exit( 1 )
	}

	// Example output:
	// Formatting 'test.qcow2', fmt=qcow2 cluster_size=65536 extended_l2=off preallocation=full compression_type=zlib size=1048576 lazy_refcounts=off refcount_bits=16

	fmt.Printf( "%s\n", output )
}

// -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd
// -drive if=pflash,format=raw,file=/mnt/raid/vm/ovmf-vars.fd
// -drive file=primary.qcow2,if=ide,cache=none,aio=native
type QEMUDrive struct {
	interface_ string

	format string
	filePath string

	isReadOnly bool

	useCache bool
	useAsyncIO bool
}

// -chardev socket,id=chrtpm,path=/mnt/raid/vm/swtpm.sock
type QEMUCharacterDevice struct {
	type_ string

	identifier string
	path string
}

// -tpmdev emulator,id=tpm0,chardev=chrtpm
type QEMUTPMDevice struct {
	type_ string

	identifier string
	characterDeviceIdentifier string
}

type QEMUDevice struct {
	type_ string
}

// -boot c
// -boot menu=on,order=cd,once=c
type QEMUBootOptions struct {
	showMenu bool
	bootOrder []string
	bootOnce string
}

type QEMUSystemOptions struct {
	// -enable-kvm
	useKVM bool

	// -machine type=q35,accel=kvm
	machineType string
	machineAccelerator string

	// -cpu host -smp cores=4
	mimicHostProcessor bool
	processorCores int

	// -m 4096M
	memoryInMiB int

	// -bios /usr/share/ovmf/OVMF.fd
	biosFilePath string

	// -drive ...
	drives []QEMUDrive

	// -chardev ...
	characterDevices []QEMUCharacterDevice

	// -tpmdev ...
	tpmDevice QEMUTPMDevice

	// -device ...
	devices []QEMUDevice

	// -cdrom debian-12.1.0-amd64-DVD-1.iso
	cdromFilePath []string

	// -boot ...
	bootOptions QEMUBootOptions
}

func startQEMUSystem()
