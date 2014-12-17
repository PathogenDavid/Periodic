using System;

namespace TestRunner
{
    /// <summary>
    /// Extensions and utilities related to the <see cref="System.Console"/> class.
    /// </summary>
    internal static class ConsoleEx
    {
        /// <summary>
        /// Prompts the user to press any key before continuing and waits for them to press a key
        /// </summary>
        internal static void PromptForKey()
        {
            Console.WriteLine();
            Console.WriteLine("Press any key to continue..");
            Console.ReadKey(true);
        }
    }
}
