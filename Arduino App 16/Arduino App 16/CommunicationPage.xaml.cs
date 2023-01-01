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
            //This pulls the preset messages.
            Mes1.Text = Connection.GrabMes(1);
            Mes1.Text = Connection.GrabMes(2);
            Mes1.Text = Connection.GrabMes(3);

            Device.StartTimer(TimeSpan.FromSeconds(10), () =>
            {
                //this pulls the pulls the temp and humidity
                tempC.Text = Connection.ReadTemp();
                return true; // True = Repeat again, False = Stop the timer
            });


            Device.StartTimer(TimeSpan.FromSeconds(5), () =>
            {
                //this pulls the recieving message every 5 seconds
                RecMes.Text = Connection.GrabMes(4);
                return true; // True = Repeat again, False = Stop the timer
            });


        }

        //This is the send message function
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

        //This sends the preset messages.
        private void UpdateMessages(object sender, EventArgs e)
        {

            if (Connection.SendMessage(Mes1.Text + '1'))
            {
                Info1.Text = "Changed";
                if (Connection.SendMessage(Mes3.Text + '2'))
                {
                    Info2.Text = "Changed";
                    if (Connection.SendMessage(Mes3.Text + '3'))
                    {
                        Info3.Text = "Changed";

                    }
                    else
                    {

                        Info3.Text = "Failed";
                    }
                }
                else
                {

                    Info2.Text = "Failed";
                }
            }
            else
            {

                Info1.Text = "Failed";
            }
        }

    }


}