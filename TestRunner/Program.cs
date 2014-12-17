using System;
using System.Diagnostics;
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

        /// <summary>Internal state: True when the test is in a passing state.</summary>
        /// <remarks>
        /// <see cref="testIsFailed"/> takes priority over this variable.
        /// In other words, if <see cref="testIsFailed"/> and <see cref="testIsPassed"/> are both true, the test should be considered failing.
        /// </remarks>
        static bool testIsPassed = false;

        /// <summary>Internal state: True when the test is in a failed state.</summary>
        static bool testIsFailed = false;

        /// <summary>
        /// Test runner entry point
        /// </summary>
        /// <param name="args">Command line arguments passed to this program. These are parsed by <see cref="TestRunenrConfig"/>.</param>
        /// <returns>The exit code of the test runner, or (when available) the exit code of the Siftulator used to run the tests.</returns>
        static int Main(string[] args)
        {
            // Parse command line arguments into config
            TestRunnerConfig config = new TestRunnerConfig(args);
            if (config.ExitCode.HasValue)
            { return config.ExitCode.Value; }

            // Run the tests:
            string tempSifulatorOutputFile = Path.GetTempFileName();

            Process sifulator = new Process();
            sifulator.StartInfo.FileName = config.SifulatorBin;
            sifulator.StartInfo.Arguments = "-T --headless --mute --flush-logs --stdout " + tempSifulatorOutputFile + " -l " + config.TestBinary;
            sifulator.StartInfo.UseShellExecute = false;
            sifulator.Start();
            sifulator.WaitForExit();

            // Annoyingly, Sifulator insists on not using the stdout given to it, so we need to have it print to a file and read that.
            if (!File.Exists(tempSifulatorOutputFile))
            {
                Console.WriteLine("!!! Couldn't find temporary output file !!!");
            }
            else
            {
                using (StreamReader testOutputStream = new StreamReader(tempSifulatorOutputFile))
                { ParseTestOutput(testOutputStream); }

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

            ConsoleEx.PromptForKey();
            return sifulator.ExitCode;
        }

        /// <summary>
        /// Parses the output from a test run and prints relevant information to the console.
        /// </summary>
        /// <param name="testOutputStream">The stream to read test output from</param>
        static void ParseTestOutput(StreamReader testOutputStream)
        {
            testIsFailed = false;
            testIsPassed = false;

            while (!testOutputStream.EndOfStream)
            {
                string logLine = testOutputStream.ReadLine();

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
    }
}
