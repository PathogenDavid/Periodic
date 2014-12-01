using System;
using System.Diagnostics;
using System.Linq;
using System.IO;

namespace TestRunner
{
    class Program
    {
        /// <summary>Prefix for all log messages emitted by the testing framework.</summary>
        /// <remarks>It is imperative that this matches the TEST_PREFIX constant in the test framework's Test.cpp</remarks>
        const string TestPrefix = "TEST: ";

        /// <summary>Special message to signify the test failing.</summary>
        /// <remarks>It is imperative that this matches the messages printed in the test framework's main.cpp</remarks>
        const string TestFailureMagic = "!!!TEST FAILURE!!!";

        /// <summary>Special message to signify the test passing.</summary>
        /// <remarks>It is imperative that this matches the messages printed in the test framework's main.cpp</remarks>
        const string TestPassMagic = "!!!TEST PASSED!!!";
        
        /// <summary>
        /// True when an error parsing command line arguments has occurred
        /// </summary>
        static bool errorParsingCommandLineArgs = false;

        /// <summary>
        /// Prints a command line processing error to the user and sets the internal error state flag.
        /// </summary>
        /// <param name="format">A composite format string (Same as <see cref="System.Console.WriteLine(System.String, System.Object[])"/>.)</param>
        /// <param name="arg">An array of objects to write using format.</param>
        static void PrintError(string format, params object[] arg)
        {
            Console.WriteLine(format, arg);
            Console.WriteLine();
            errorParsingCommandLineArgs = true;
        }

        /// <summary>
        /// Prints a command line processing error caused by redundant switches
        /// </summary>
        /// <param name="property">The property (in plain English) that was specified redundantly.</param>
        /// <param name="currentValue">The current value for that property.</param>
        /// <param name="newValue">The value the user specified.</param>
        static void PrintDupeError(string property, string currentValue, string newValue)
        {
            Console.WriteLine("Specified {0} more than once.", property);
            Console.WriteLine("    Current {0}: '{1}'", property, currentValue);
            Console.WriteLine("        New {0}: '{1}'", property, newValue);
            Console.WriteLine();
            errorParsingCommandLineArgs = true;
        }

        static int Main(string[] args)
        {
            string sdkDir = null;
            string testBinary = null;
            const string defaultSiftultorBin = "bin/siftulator.exe";
            string sifulatorBin = null;

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
                    else if (testBinary != null)
                    {
                        PrintDupeError("test binary", testBinary, rawArg);
                    }
                    else
                    {
                        testBinary = rawArg;
                    }

                    continue;
                }
                else if (currentSwitch == "sifulatorbin")
                {
                    if (sifulatorBin != null)
                    {
                        PrintDupeError("sifulator binary", sifulatorBin, rawArg);
                    }
                    else
                    {
                        sifulatorBin = rawArg;
                    }
                }
                else if (currentSwitch == "sdkdir")
                {
                    if (sdkDir != null)
                    {
                        PrintDupeError("SDK directory", sdkDir, rawArg);
                    }
                    else
                    {
                        sdkDir = rawArg;
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
            if (sdkDir == null)
            { sdkDir = Environment.GetEnvironmentVariable("SDK_DIR") ?? ""; } // Use current workding directory is SDK_DIR is not available

            if (sifulatorBin == null)
            { sifulatorBin = defaultSiftultorBin; }

            // Validate input
            sifulatorBin = Path.Combine(sdkDir, sifulatorBin);

            if (!File.Exists(sifulatorBin))
            { PrintError("Sifulator binary '{0}' doesn't exist!", sifulatorBin); }

            if (testBinary == null)
            { PrintError("No test binary specified."); }
            else if (!File.Exists(testBinary))
            { PrintError("Test binary '{0}' could not be found.", testBinary); }

            // Print help if necessary
            if (printHelp || errorParsingCommandLineArgs)
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

                PressAnyKey();
                return errorParsingCommandLineArgs ? 1 : 0;
            }

            // Run the tests:
            string tempSifulatorOutputFile = Path.GetTempFileName();

            Process sifulator = new Process();
            sifulator.StartInfo.FileName = sifulatorBin;
            sifulator.StartInfo.Arguments = "-T --headless --mute --flush-logs --stdout " + tempSifulatorOutputFile + " -l " + testBinary;
            sifulator.StartInfo.UseShellExecute = false;
            sifulator.Start();
            sifulator.WaitForExit();

            // Annoyingly, Sifulator insists on not using the stdout given to it, so we need to have it print to a file and read that.
            bool testIsPassed = false;
            bool testIsFailed = false;
            if (!File.Exists(tempSifulatorOutputFile))
            {
                Console.WriteLine("!!! Couldn't find temporary output file !!!");
            }
            else
            {
                using (StreamReader f = new StreamReader(tempSifulatorOutputFile))
                {
                    ParseTestOutputFile(f, out testIsFailed, out testIsPassed);
                }

                File.Delete(tempSifulatorOutputFile);
            }

            Console.WriteLine();

            if (sifulator.ExitCode != 0)
            {
                //                 !!!!____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____!!!!
                Console.WriteLine("!!! Siftulator exited with an error code. Test failure forced !!!");
                Console.WriteLine();
                testIsFailed = true;
            }

            if (testIsPassed && !testIsFailed)
            {
                Console.WriteLine(@" _____         _____ _____ ");
                Console.WriteLine(@"|  __ \ /\    / ____/ ____|");
                Console.WriteLine(@"| |__) /  \  | (___| (___  ");
                Console.WriteLine(@"|  ___/ /\ \  \___ \\___ \ ");
                Console.WriteLine(@"| |  / ____ \ ____) |___) |");
                Console.WriteLine(@"|_| /_/    \_\_____/_____/ ");
            }
            else
            {
                if (!testIsFailed)
                {
                    Console.WriteLine("ERROR: The testing framework never emitted test verdict!");
                }

                Console.WriteLine(@" ______      _____ _      ");
                Console.WriteLine(@"|  ____/\   |_   _| |     ");
                Console.WriteLine(@"| |__ /  \    | | | |     ");
                Console.WriteLine(@"|  __/ /\ \   | | | |     ");
                Console.WriteLine(@"| | / ____ \ _| |_| |____ ");
                Console.WriteLine(@"|_|/_/    \_\_____|______|");
            }

            PressAnyKey();
            return sifulator.ExitCode;
        }

        /// <summary>
        /// Parses the output from a test run
        /// </summary>
        /// <param name="f">The stream to read test output from</param>
        /// <param name="testIsFailed">Indicates if the test has failed (overrides testIsPassed</param>
        /// <param name="testIsPassed">Indicates if the test has passed</param>
        static void ParseTestOutputFile(StreamReader f, out bool testIsFailed, out bool testIsPassed)
        {
            testIsFailed = false;
            testIsPassed = false;

            while (!f.EndOfStream)
            {
                string logLine = f.ReadLine();

                // Ignore lines that aren't from the test framework (Probably stuff emitted by Siftulator.)
                if (!logLine.StartsWith(TestPrefix))
                {
                    continue;
                }

                // Strip the test prefix:
                logLine = logLine.Remove(0, TestPrefix.Length);

                // Assert that the failed/passed magic message is the last message sent by the test framework.
                if (testIsFailed || testIsPassed)
                {
                    //                 !!!!____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____80 CHARACTERS____!!!!
                    Console.WriteLine("!!! WARNING: Test framework emitted messages after test passing/failure !!!");
                    Console.WriteLine("!!!            Test failure forced by test running utility.             !!!");
                    testIsFailed = true;
                }
                else if (logLine == TestFailureMagic)
                {
                    testIsFailed = true;
                }
                else if (logLine == TestPassMagic)
                {
                    testIsPassed = true;
                }

                // Print the line:
                Console.WriteLine(logLine);
            }
        }

        /// <summary>
        /// Prompts the user to press any key before continuing and waits for them to press a key
        /// </summary>
        static void PressAnyKey()
        {
            Console.WriteLine();
            Console.WriteLine("Press any key to continue..");
            Console.ReadKey(true);
        }
    }
}
