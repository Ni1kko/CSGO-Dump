using System;
using System.Diagnostics;

namespace DDX.Core
{
    public class Engine : IDisposable
    {
        private readonly Process _gameProcess;

        public ThreadManager ThreadManager;

        private bool _working;

        public Engine(Process process)
        {
            _gameProcess = process;
        }

        public void Dispose()
        {
            Stop();
        }

        public bool Start()
        {
            if (_working) return true;

            ProcessModuleCollection gameModules;

            try
            {
                gameModules = _gameProcess.Modules;
            }
            catch
            {
                return false;
            }

            Module.Client = null;
            Module.Engine = null;

            // Scan modules
            foreach (ProcessModule module in gameModules)
            {
                if (Module.Client != null &&
                    Module.Engine != null)
                {
                    break;
                }

                switch (module.ModuleName)
                {
                    case "client.dll":
                        Module.Client = module;
                        break;
                    case "engine.dll":
                        Module.Engine = module;
                        break;
                }
            }

            // Check if scan was successful
            if (Module.Client == null ||
                Module.Engine == null)
                return false;

            // Create objects
            G.Mem = new Memory(_gameProcess);
            new SignatureScanner().Scan();
            ThreadManager = new ThreadManager();

            _working = true;

            return true;
        }

        public void Stop()
        {
            if (!_working) return;

            ThreadManager.Dispose();
            G.Mem.Dispose();

            _working = false;
        }
    }
}
