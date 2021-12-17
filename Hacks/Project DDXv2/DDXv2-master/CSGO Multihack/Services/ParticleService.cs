using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DDX.Structs;
using DDX.Views;

namespace DDX.Services
{
    public class ParticleService : IDisposable
    {
        private const int ParticleCount = 80;
        private const float ParticleSize = 1f;
        private const float ConnectionSize = 1f;
        private const float CursorConnectionSize = .6f;
        private const float ConnectionStart = 40f;
        private const float ConnectionEnd = 80f;
        private const float ConnectionPerc = (ConnectionEnd - ConnectionStart) / 100f;
        private const int FpsCap = 30;

        private static readonly Color ConnectionColor = Colors.Black;
        private static readonly Color CursorConnectionColor = Color.FromRgb(51, 180, 255);
        private static readonly Color ParticleColor = Color.FromRgb(51, 180, 255);

        private static readonly SolidColorBrush ConnectionBrush = new SolidColorBrush(ConnectionColor);
        private static readonly SolidColorBrush CursorConnectionBrush = new SolidColorBrush(CursorConnectionColor);
        private static readonly SolidColorBrush ParticleBrush = new SolidColorBrush(ParticleColor);

        private static readonly Pen ConnectionPen = new Pen(ConnectionBrush, ConnectionSize);
        private static readonly Pen CursorConnectionPen = new Pen(CursorConnectionBrush, CursorConnectionSize);

        private readonly LoaderWindow _window;
        private readonly Random _random;

        private readonly double _dpiX;
        private readonly double _dpiY;

        private Particle[] _particles;
        private Thread _thread;
        private bool _working;

        public ParticleService(LoaderWindow window)
        {
            _random = new Random();
            _window = window;

            var source = PresentationSource.FromVisual(window);
            _dpiX = 96f * source.CompositionTarget.TransformToDevice.M11;
            _dpiY = 96f * source.CompositionTarget.TransformToDevice.M22;
        }

        public void Dispose()
        {
            Stop();
        }

        public void Start()
        {
            if (_working) return;

            #region Generate particles
            
            _particles = new Particle[ParticleCount];

            for (var i = 0; i < _particles.Length; i++)
            {
                _particles[i] = new Particle()
                {
                    X = _random.Next(0, (int)_window.Width + 1),
                    Y = _random.Next(0, (int)_window.Height + 1),

                    YSpeed = _random.Next(5, 20 + 1) / 100f,
                    XSpeed = _random.Next(5, 20 + 1)/100f,

                    GoingRight = _random.Next(0, 1 + 1) == 1,
                    GoingDown = _random.Next(0, 1 + 1) == 1,
                };
            }
            
            #endregion

            _thread = new Thread(Service) {IsBackground = true};
            _thread.Start();

            _working = true;
        }

        public void Stop()
        {
            if (!_working) return;

            _thread?.Abort();

            _working = false;
        }

        private void Service()
        {
            var stopwatch = new Stopwatch();
            stopwatch.Start();

            var drawWidth = 0d;
            var drawHeight = 0d;

            _window.Dispatcher.Invoke(() =>
            {
                drawWidth = _window.Width;
                drawHeight = _window.Height - _window.TitlebarHeight;
            });

            while (true)
            {
                #region Animate

                for (var i = 0; i < _particles.Length; i++)
                {
                    #region Horizontal

                    // Move
                    _particles[i].X = _particles[i].GoingRight
                        ? _particles[i].X + _particles[i].XSpeed
                        : _particles[i].X - _particles[i].XSpeed;

                    // Change direction
                    if (_particles[i].GoingRight && _particles[i].X > drawWidth)
                    {
                        _particles[i].X = (float)drawWidth;
                        _particles[i].GoingRight = false;
                    }
                    else if (!_particles[i].GoingRight && _particles[i].X < 0)
                    {
                        _particles[i].X = 0;
                        _particles[i].GoingRight = true;
                    }

                    #endregion

                    #region Vertical

                    // Move
                    _particles[i].Y = _particles[i].GoingDown
                        ? _particles[i].Y + _particles[i].YSpeed
                        : _particles[i].Y - _particles[i].YSpeed;

                    // Change direction
                    if (_particles[i].GoingDown && _particles[i].Y > drawHeight)
                    {
                        _particles[i].Y = (float)drawHeight;
                        _particles[i].GoingDown = false;
                    }
                    else if (!_particles[i].GoingDown && _particles[i].Y < 0)
                    {
                        _particles[i].Y = 0;
                        _particles[i].GoingDown = true;
                    }

                    #endregion
                }

                #endregion

                try
                {
                    _window.Dispatcher.Invoke(() =>
                    {
                        var drawingVisual = new DrawingVisual();
                        var drawingContext = drawingVisual.RenderOpen();

                        #region Draw Connections

                        // Particle-Particle
                        for (var i = 0; i < ParticleCount; i++)
                        {
                            for (var j = i + 1; j < ParticleCount; j++)
                            {
                                var distance = Distance(
                                    _particles[i].X, _particles[i].Y,
                                    _particles[j].X, _particles[j].Y);

                                Pen pen;

                                if (distance > ConnectionEnd)
                                    continue;
                                if (distance < ConnectionStart)
                                    pen = ConnectionPen;
                                else
                                {
                                    var distancePerc = (distance - ConnectionStart) / ConnectionPerc;
                                    var distancePercByte = 255 - distancePerc * 2.55f;

                                    var colorTmp = ConnectionColor;
                                    colorTmp.A = (byte)distancePercByte;

                                    pen = new Pen(new SolidColorBrush(colorTmp), ConnectionSize);
                                }

                                drawingContext.DrawLine(pen,
                                    new Point(_particles[i].X, _particles[i].Y),
                                    new Point(_particles[j].X, _particles[j].Y));
                            }
                        }

                        var cursorPos = Mouse.GetPosition(_window.ParticleImage);

                        // Cursor-Particle
                        foreach (var particle in _particles)
                        {
                            var distance = Distance(
                                (float)cursorPos.X, (float)cursorPos.Y,
                                particle.X, particle.Y);

                            Pen pen;

                            if (distance > ConnectionEnd)
                                continue;
                            if (distance < ConnectionStart)
                                pen = CursorConnectionPen;
                            else
                            {
                                var distancePerc = (distance - ConnectionStart) / ConnectionPerc;
                                var distancePercByte = 255 - distancePerc * 2.55f;

                                var colorTmp = CursorConnectionColor;
                                colorTmp.A = (byte)distancePercByte;

                                pen = new Pen(new SolidColorBrush(colorTmp), CursorConnectionSize);
                            }

                            drawingContext.DrawLine(pen,
                                new Point((float)cursorPos.X, (float)cursorPos.Y),
                                new Point(particle.X, particle.Y));
                        }

                        #endregion

                        #region Draw Particles

                        foreach (var particle in _particles)
                        {
                            drawingContext.DrawEllipse(ParticleBrush, null,
                                new Point(particle.X, particle.Y), ParticleSize,
                                ParticleSize);
                        }

                        #endregion

                        drawingContext.Close();

                        var bmp = new RenderTargetBitmap((int)drawWidth, (int)drawHeight, _dpiX, _dpiY, PixelFormats.Pbgra32);
                        bmp.Render(drawingVisual);

                        _window.ParticleImage.Source = bmp;
                    });
                }
                catch (TaskCanceledException)
                { }
                
                #region FpsCap

                var frameLength = stopwatch.ElapsedMilliseconds;
                var frameDifference = 1000f / FpsCap - frameLength;

                if (frameDifference > 0)
                {
                    Thread.Sleep((int)frameDifference);
                }

                #endregion

                stopwatch.Restart();
            }
        }

        private float Distance(float x1, float y1, float x2, float y2)
        {
            return (float) Math.Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        }
    }
}
