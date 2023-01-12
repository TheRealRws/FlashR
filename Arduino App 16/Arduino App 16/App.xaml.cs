using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace Arduino_App_16
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();

            MainPage = new NavigationPage (new MainPage());
        }
        public static int ScreenHeight { get; set; }
        public static int ScreenWidth { get; set; }
        protected override void OnStart()
        {
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }
    }
}
