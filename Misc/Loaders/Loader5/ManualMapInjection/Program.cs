using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using ManualMapInjection.Injection;
using System.Threading;
using System.Net;

namespace ManualMapInjection
{
    class Program
    {
        static void Main(string[] args)
        {

            Beginning:

            Console.Title = "Nova Hook | Login";

            Console.ForegroundColor = ConsoleColor.White;

            string login;
            string password;

            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine("    _   __                    __  __            __  ");
            Console.WriteLine("   / | / /___ _   ______ _   / / / /___  ____  / /__");
            Console.WriteLine("  /  |/ / __ \\ | / / __ `/  / /_/ / __ \\/ __ \\/ //_/");
            Console.WriteLine(" / /|  / /_/ / |/ / /_/ /  / __  / /_/ / /_/ / ,<   ");
            Console.WriteLine("/_/ |_/\\____/|___/\\__,_/  /_/ /_/\\____/\\____/_/|_| ");
            Console.WriteLine("");
            Console.WriteLine("");
            Console.ForegroundColor = ConsoleColor.White;

            Console.Write("Username: ");
            login = Console.ReadLine();

            Console.Write("Password: ");
            password = Console.ReadLine();

            WebRequest req = WebRequest.Create("http://localhost/check.php?username=" + login + "&password=" + password);

            WebResponse res = req.GetResponse();
            StreamReader sr = new StreamReader(res.GetResponseStream());
            string returnvalue = sr.ReadToEnd();

            if (returnvalue.Contains("2"))
            {
                Console.Clear();
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Password incorrect");
                Thread.Sleep(1000);
                Console.Clear();
                goto Beginning;
            }
            else if (returnvalue.Contains("1"))
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("Password accepted");
                Thread.Sleep(1000);
                Console.Clear();
                Console.ForegroundColor = ConsoleColor.White;
                goto loggedin;
            }

            loggedin:

            Console.Title = "Nova Hook | Logged in";
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine("    _   __                    __  __            __  ");
            Console.WriteLine("   / | / /___ _   ______ _   / / / /___  ____  / /__");
            Console.WriteLine("  /  |/ / __ \\ | / / __ `/  / /_/ / __ \\/ __ \\/ //_/");
            Console.WriteLine(" / /|  / /_/ / |/ / /_/ /  / __  / /_/ / /_/ / ,<   ");
            Console.WriteLine("/_/ |_/\\____/|___/\\__,_/  /_/ /_/\\____/\\____/_/|_| ");
            Console.WriteLine("");
            Console.WriteLine("");
            Console.ForegroundColor = ConsoleColor.White;

            Console.WriteLine("Please open CS:GO now!");
            goto gamecheck;

            gamecheck:
            var name = "csgo";
            var target = Process.GetProcessesByName(name).FirstOrDefault();
            if (target == null)
            {
                Thread.Sleep(500);
                goto gamecheck;
            }

            injector:
            Thread.Sleep(0); // Delay injections here
            Console.Clear();
            Console.Title = "Nova Hook";
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine("    _   __                    __  __            __  ");
            Console.WriteLine("   / | / /___ _   ______ _   / / / /___  ____  / /__");
            Console.WriteLine("  /  |/ / __ \\ | / / __ `/  / /_/ / __ \\/ __ \\/ //_/");
            Console.WriteLine(" / /|  / /_/ / |/ / /_/ /  / __  / /_/ / /_/ / ,<   ");
            Console.WriteLine("/_/ |_/\\____/|___/\\__,_/  /_/ /_/\\____/\\____/_/|_| ");
            Console.WriteLine("");

            if (target == null)
            {
                Console.Title = "Nova Hook | Status: PROCESS MISSING";
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("[Nova Hook] Error: CS:GO process does not exist!");
                Thread.Sleep(500);
                goto gamecheck;
            }

            var path = "Nova Hook.dll";

            if (!File.Exists(path))
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("[Nova Hook] Error: DLL does not exist! Make sure it's named \"Nova Hook.dll\"");
                Thread.Sleep(500);
                goto injector;
            }
            var file = File.ReadAllBytes(path);

            var injector = new ManualMapInjector(target) {AsyncInjection = true};
            Console.ForegroundColor = ConsoleColor.Black;
            Console.WriteLine($"[Nova Hook] hmodule = 0x{injector.Inject(file).ToInt64():x8}");
        }
    }
}
