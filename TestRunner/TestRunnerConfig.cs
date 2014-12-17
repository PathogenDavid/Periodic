using System;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace TestRunner
{
    /// <summary>
    /// Parses and handles the configuration for the test runner.
    /// It is also responsible for printing errors about command line arguments and help information to the user.
    /// </summary>
    /// <remarks>
    /// When used, make sure to check the value of <see cref="TestRunnerConfig.ExitCode"/> to ensure the configuration is valid.
    /// </remarks>
    internal class TestRunnerConfig
    {
        /// <summary>
        /// Path to the Sifteo SDK directory
        /// </summary>
        /// <remarks>If <see cref="TestRunnerConfig.ExitCode"/> has a value, the value of this property is undefined.</remarks>
        public string SdkDir { get; private set; }

        /// <summary>
        /// Path to the test binary to load into the Siftulator
        /// </summary>
        /// <remarks>If <see cref="TestRunnerConfig.ExitCode"/> has a value, the value of this property is undefined.</remarks>
        public string TestBinary { get; private set; }

        /// <summary>
        /// Path to the sifulator binary
        /// </summary>
        /// <remarks>If <see cref="TestRunnerConfig.ExitCode"/> has a value, the value of this property is undefined.</remarks>
        public string SifulatorBin { get; private set; }

        /// <summary>
        /// Null when the program should continue execution after constructing the test runner config.
        /// Has value to return to the OS as an exit code when execution should not continue.
        /// </summary>
        /// <remarks>If this has a value, the configuration information is in an undefined state.</remarks>
        public int? ExitCode { get; private set; }
        
        /// <summary>
        /// True when an error parsing command line arguments has occurred
        /// </summary>
        public bool ErrorParsingCommandLineArgs { get; private set; }

        /// <summary>
        /// Parses the given command line arguments and configures the internal state of the program
        /// </summary>
        /// <param name="args">The arguments to parse</param>
        /// <param name="returnCode">The return code to send to the OS upon failure</param>
        /// <returns>True when the program should continue, false when the program should exit with the given return code.</returns>
        public TestRunnerConfig(string[] args)
        {
            const string defaultSiftultorBin = "bin/siftulator.exe";

            // Must all be lowercase
            string[] switches =
            {
                "sdkdir",
                "sifulatorbin"
            };

            // Parse command line args:
            bool printHelp = false;
            string currentSwitch = null;
            foreach (string rawArg in args)
            {
                string arg = rawArg;

                // If we don't have a switch:
                if (currentSwitch == null)
                {
                    bool isSwitch = false;

                    if (arg.StartsWith("--"))
                    {
                        arg = arg.Remove(0, 2);
                        isSwitch = true;
                    }
                    else if (arg.StartsWith("-"))
                    {
                        arg = arg.Remove(0, 1);
                        isSwitch = true;
                    }

                    arg = arg.ToLowerInvariant();

                    // Ignore testbin switch: (Its value is handled below since it can be floating)
                    if (isSwitch && arg == "testbin")
                    {
                        continue;
                    }

                    if (isSwitch)
                    {
                        if (switches.Contains(arg.ToLowerInvariant()))
                        {
                            currentSwitch = arg;
                        }
                        else
                        {
                            PrintError("Error: '{0}' is not a valid switch!", rawArg);
                        }
                    }
                    // Floating arguments are the test binary
                    else if (TestBinary != null)
                    {
                        PrintDupeError("test binary", TestBinary, rawArg);
                    }
                    else
                    {
                        TestBinary = rawArg;
                    }

                    continue;
                }
                else if (currentSwitch == "sifulatorbin")
                {
                    if (SifulatorBin != null)
                    {
                        PrintDupeError("sifulator binary", SifulatorBin, rawArg);
                    }
                    else
                    {
                        SifulatorBin = rawArg;
                    }
                }
                else if (currentSwitch == "sdkdir")
                {
                    if (SdkDir != null)
                    {
                        PrintDupeError("SDK directory", SdkDir, rawArg);
                    }
                    else
                    {
                        SdkDir = rawArg;
                    }
                }
                else
                {
                    PrintError("FATAL: Internal error while parsing command line arguments.");
                    Debug.Assert(false);
                }

                // Reset current switch (Skipped when we just got a switch.)
                currentSwitch = null;
            }

            // Apply defaults if necessary:
            if (SdkDir == null)
            { SdkDir = Environment.GetEnvironmentVariable("SDK_DIR") ?? ""; } // Use current workding directory is SDK_DIR is not available

            if (SifulatorBin == null)
            { SifulatorBin = defaultSiftultorBin; }

            // Validate input
            SifulatorBin = Path.Combine(SdkDir, SifulatorBin);

            if (!File.Exists(SifulatorBin))
            { PrintError("Sifulator binary '{0}' doesn't exist!", SifulatorBin); }

            if (TestBinary == null)
            { PrintError("No test binary specified."); }
            else if (!File.Exists(TestBinary))
            { PrintError("Test binary '{0}' could not be found.", TestBinary); }

            // Print help if necessary
            if (printHelp || ErrorParsingCommandLineArgs)
            {
                //                 !!!!____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____!!!!
                Console.WriteLine("Basic usage: TestRunner testbin.elf");
                Console.WriteLine();
                Console.WriteLine("=== Command Line Switch Details ===");
                Console.WriteLine("--sdkdir <sdk directory>");
                Console.WriteLine("    Explicitly specify Sifteo SDK directory");
                Console.WriteLine("    (Default is the value of the SDK_DIR environment variable.)");
                Console.WriteLine("--siftulatorBin <siftulator>");
                Console.WriteLine("    Explicitly specify Sifulator bin, relative to Sifteo SDK directory.");
                Console.WriteLine("    (Default is '{0}')", defaultSiftultorBin);
                Console.WriteLine("--testbin <testBinary.elf>");
                Console.WriteLine("    Alternate method for specifying test binary");
                Console.WriteLine("--help -h - Print this help");

                ConsoleEx.PromptForKey();
                ExitCode = ErrorParsingCommandLineArgs ? 1 : 0;
            }

            ExitCode = null;
        }

        /// <summary>
        /// Prints a command line processing error to the user and sets the internal error state flag.
        /// </summary>
        /// <param name="format">A composite format string (Same as <see cref="System.Console.WriteLine(System.String, System.Object[])"/>.)</param>
        /// <param name="arg">An array of objects to write using format.</param>
        private void PrintError(string format, params object[] arg)
        {
            Console.WriteLine(format, arg);
            Console.WriteLine();
            ErrorParsingCommandLineArgs = true;
        }

        /// <summary>
        /// Prints a command line processing error caused by redundant switches
        /// </summary>
        /// <param name="property">The property (in plain English) that was specified redundantly.</param>
        /// <param name="currentValue">The current value for that property.</param>
        /// <param name="newValue">The value the user specified.</param>
        private void PrintDupeError(string property, string currentValue, string newValue)
        {
            Console.WriteLine("Specified {0} more than once.", property);
            Console.WriteLine("    Current {0}: '{1}'", property, currentValue);
            Console.WriteLine("        New {0}: '{1}'", property, newValue);
            Console.WriteLine();
            ErrorParsingCommandLineArgs = true;
        }
    }
}
