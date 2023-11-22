package main

import "flag"
import "fmt"
import "os"

const PROJECT_NAME string = "Virtual Machine Manager"
const PROJECT_VERSION string = "0.1.0"

const AUTHOR_NAME string = "viral32111"
const AUTHOR_WEBSITE string = "https://viral32111.com"

const REPOSITORY_URL string = "https://github.com/viral32111/virtual-machine-manager"

const ENVIRONMENT_VARIABLE_PREFIX string = "VMM"

// Centers text by padding it with spaces.
func centerText( format string, maxColumns int, values ...any ) string {
	var text string = fmt.Sprintf( format, values... )
	var paddingLength int = ( maxColumns + len( text ) ) / 2

	return fmt.Sprintf( "%*s", paddingLength, text )
}

// Prefixes an environment variable name with the project's name.
func getEnvironmentVariableName( name string ) string {
	return fmt.Sprintf( "%s_%s", ENVIRONMENT_VARIABLE_PREFIX, name )
}

func getFlagUsage( description string, environmentVariableName string ) string {
	return fmt.Sprintf( "%s [%s]", description, getEnvironmentVariableName( environmentVariableName ) )
}

func printHelp() {
	var executablePath string = os.Args[ 0 ]

	var longestTextLength int = len( REPOSITORY_URL )
	fmt.Fprintln( os.Stderr, centerText( "%s (v%s).", longestTextLength, PROJECT_NAME, PROJECT_VERSION ) )
	fmt.Fprintln( os.Stderr, centerText( "Created by %s (%s).", longestTextLength, AUTHOR_NAME, AUTHOR_WEBSITE ) )
	fmt.Fprintln( os.Stderr, REPOSITORY_URL )

	fmt.Fprintf( os.Stderr, "\nUsage: %s [-config <path>] <action>\n", executablePath )
	fmt.Fprintf( os.Stderr, "Example: %s -config email.yaml start\n", executablePath )

	fmt.Fprintln( os.Stderr, "\nFlags:" )
	flag.PrintDefaults()

	fmt.Fprintln( os.Stderr, "\nActions:" )
	fmt.Fprintln( os.Stderr, "\tstart: Starts the virtual machine." )
	fmt.Fprintln( os.Stderr, "\tstop: Stops the virtual machine." )

	os.Exit( 1 )
}

func main() {
	var configurationFilePath string = "example.yaml"

	flag.StringVar( &configurationFilePath, "config", configurationFilePath, getFlagUsage( "The virtual machine configuration file.", "CONFIG_FILE_PATH" ) )
	flag.Usage = printHelp

	configurationFilePathEnvironmentVariable, doesNotExist := os.LookupEnv( getEnvironmentVariableName( "CONFIG_FILE_PATH" ) )
	if ( !doesNotExist && len( configurationFilePathEnvironmentVariable ) > 0 ) {
		configurationFilePath = configurationFilePathEnvironmentVariable
	}

	flag.Parse()

	//fmt.Println( "Configuration File:", configurationFilePath )

	if flag.NArg() != 1 {
		flag.Usage()
	}

	var action string = flag.Arg( 0 )
	fmt.Println( "Action:", action )

	os.Exit( 0 )
}
