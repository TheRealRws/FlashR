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
    public partial class CommunicationPage : ContentPage
    {


        public CommunicationPage()
        {

            InitializeComponent();
            Device.StartTimer(TimeSpan.FromSeconds(10), () =>
            {
                // Do something
                tempC.Text = Connection.ReadTemp().ToString();
                return true; // True = Repeat again, False = Stop the timer
            });




        }

        private void Toggle_LED_Clicked(object sender, EventArgs e)
        {

            if (Connection.SendMessage(InputText.Text+ '$'))
            {
                recieved.Text ="Message Recieved";
            }else
            {
                recieved.Text = "Error";

            }
        }
        

    }


}