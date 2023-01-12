using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using System.Timers;

namespace Arduino_App_16
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class temperaturePage : ContentPage
    {
        public temperaturePage()
        {
            InitializeComponent();

            Device.StartTimer(TimeSpan.FromSeconds(10), () =>
            {
                //this pulls the pulls the temp and humidity
                TempC.Text = Connection.ReadTemp();
                return true; // True = Repeat again, False = Stop the timer
            });
        }
    }
}