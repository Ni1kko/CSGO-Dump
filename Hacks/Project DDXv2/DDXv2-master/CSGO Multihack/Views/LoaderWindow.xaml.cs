using System;
using System.Diagnostics;
using System.Threading;
using System.Windows;
using DDX.Core;
using DDX.Forms;
using DDX.Services;
using MessageBox = System.Windows.MessageBox;

namespace DDX.Views
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class LoaderWindow
    {
        private ParticleService _particleService;
        private KeyboardService _keyboardService;

        private bool _fireExit;

        public LoaderWindow()
        {
            InitializeComponent();

            Title = "DDX Multihack";
            VersionText.Text = $"v{Settings.AppVersion}";

            Loaded += OnLoad;
        }

        private void OnLoad(object sender, RoutedEventArgs e)
        {
            // Background animation
            _particleService = new ParticleService(this);
            _particleService.Start();
        }

        public void StartButton_OnClick(object sender, RoutedEventArgs e)
        {
            #region Check process

            var gameProcesses = Process.GetProcessesByName("csgo");
            if (gameProcesses.Length == 0)
            {
                MessageBox.Show("Game process not found.\nMake sure that CS:GO is running.", "DDX Multihack", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            var gameProcess = gameProcesses[0];

            #endregion

            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.AboveNormal;
            gameProcess.PriorityClass = ProcessPriorityClass.BelowNormal;

            #region Run cheat

            // Hide this window
            Hide();

            _particleService.Dispose();

            ConfigParser.Load();

            // Show overlay and menu
            G.O = new OverlayForm();
            G.O.Show();

            var mainWindow = new MenuWindow();
            mainWindow.Show();

            _fireExit = false;

            // Add close events to show this window again and stop services
            G.O.Closed += Overlay_OnWindowClosed;
            mainWindow.Closed += Overlay_OnWindowClosed;

            // Start keyboard service
            _keyboardService = new KeyboardService(mainWindow);
            _keyboardService.Start();

            var thr2 = new Thread(() =>
            {
                var engine = new Engine(gameProcess);

                // Game not found, let's try again
                while (!engine.Start())
                {
                    Thread.Sleep(200);
                }

                // Start cheat threads
                engine.ThreadManager.Start();

                GC.Collect();

                while (!gameProcess.HasExited && !_fireExit)
                {
                    // Do cheat
                    Thread.Sleep(50);
                }

                GC.Collect();

                // Stop cheat and engine
                engine.ThreadManager.Stop();
                engine.Stop();

                Dispatcher.Invoke(() =>
                {
                    // Remove close events
                    G.O.Closed -= Overlay_OnWindowClosed;
                    mainWindow.Closed -= Overlay_OnWindowClosed;

                    // Stop services
                    _keyboardService.Dispose();

                    mainWindow.Close();
                    mainWindow = null;

                    G.O.Close();
                    G.O = null;

                    // Show this window again
                    _particleService = new ParticleService(this);
                    _particleService.Start();

                    Show();
                    Activate();

                    GC.Collect();
                });
            }) {IsBackground = true};
            thr2.Start();

            #endregion
        }
        private void Overlay_OnWindowClosed(object sernder, EventArgs e)
        {
            _fireExit = true;
        }
    }
}