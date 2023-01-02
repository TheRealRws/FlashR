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

        int x = 1;
        public CommunicationPage()
        {

            InitializeComponent();
            //This pulls the preset messages.



            Device.StartTimer(TimeSpan.FromSeconds(10), () =>
            {
                //this pulls the pulls the temp and humidity
                tempC.Text = Connection.ReadTemp();
                return true; // True = Repeat again, False = Stop the timer
            });


            Device.StartTimer(TimeSpan.FromSeconds(31), () =>
            {
                //this pulls the recieving message every 5 seconds
                RecMes.Text = Connection.GrabMes(4);
                return true; // True = Repeat again, False = Stop the timer
            });



            Device.StartTimer(TimeSpan.FromSeconds(3), () =>
            {
                //this pulls the pulls the temp and humidity

                if (x == 1)
                {
                    Mes1.Text = Connection.GrabMes(x);

                    x++;
                    return true;
                }
                if (x == 2)
                {

                    Mes2.Text = Connection.GrabMes(x);

                    x++;
                    return true;
                }
                if (x == 3)
                {

                    Mes3.Text = Connection.GrabMes(x);

                    return false;
                }

                return true;
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

            if (Connection.SendMessage(Mes1.Text + "1#"))
            {
                Info1.Text = "Changed";
                if (Connection.SendMessage(Mes2.Text + "2#"))
                {
                    Info2.Text = "Changed";
                    if (Connection.SendMessage(Mes3.Text + "3#"))
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