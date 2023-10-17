package main

import (
	"flag"
	"fmt"
	"os"
)

const PROJECT_NAME string = "Virtual Machine Manager"
const PROJECT_VERSION string = "0.1.0"

const AUTHOR_NAME string = "viral32111"
const AUTHOR_WEBSITE string = "https://viral32111.com"

const REPOSITORY_URL string = "https://github.com/viral32111/virtual-machine-manager"

const ENVIRONMENT_VARIABLE_PREFIX string = "VM"

func printCentered( format string, maxColumns int, values ...any ) {
	var text string = fmt.Sprintf( format, values... )
	var paddingLength int = ( maxColumns + len( text ) ) / 2

	fmt.Printf( "%*s\n", paddingLength, text )
}

func generateEnvironmentVariableName( name string ) string {
	return fmt.Sprintf( "%s_%s", ENVIRONMENT_VARIABLE_PREFIX, name )
}

func generateFlagUsageText( description string, environmentVariableName string ) string {
	return fmt.Sprintf( "%s [%s]", description, generateEnvironmentVariableName( environmentVariableName ) )
}

func printFlagUsage() {
	var executablePath string = os.Args[ 0 ]

	var longestTextLength int = len( REPOSITORY_URL )
	printCentered( "%s (v%s).", longestTextLength, PROJECT_NAME, PROJECT_VERSION )
	printCentered( "Created by %s (%s).", longestTextLength, AUTHOR_NAME, AUTHOR_WEBSITE )
	fmt.Println( REPOSITORY_URL )

	fmt.Printf( "\nUsage: %s [-config <path>] <action>\n", executablePath )
	flag.PrintDefaults()
	fmt.Println( "\nActions: start, stop" )
	fmt.Printf( "\nExample: %s -config email.yaml start", executablePath )

	os.Exit( 1 )
}

func main() {
	var configurationFilePath string = "example.yaml"

	flag.StringVar( &configurationFilePath, "config", configurationFilePath, generateFlagUsageText( "The virtual machine configuration file.", "CONFIG_FILE_PATH" ) )
	flag.Usage = printFlagUsage

	configurationFilePathEnvironmentVariable, doesNotExist := os.LookupEnv( generateEnvironmentVariableName( "CONFIG_FILE_PATH" ) )
	if ( !doesNotExist && len( configurationFilePathEnvironmentVariable ) > 0 ) {
		configurationFilePath = configurationFilePathEnvironmentVariable
	}

	flag.Parse()

	fmt.Println( "Configuration File:", configurationFilePath )

	if flag.NArg() != 1 {
		flag.Usage()
	}

	var action string = flag.Arg( 0 )
	fmt.Println( "Action:", action )

	os.Exit( 0 )
}
