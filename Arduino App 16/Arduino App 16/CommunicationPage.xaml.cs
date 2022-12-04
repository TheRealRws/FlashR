using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace Arduino_App_16
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CommunicationPage : ContentPage
    {
        bool MessageRecieved = false;

        public CommunicationPage()
        {
            InitializeComponent();
        }

        private void Toggle_LED_Clicked(object sender, EventArgs e)
        {

            if (Connection.SendMessage(input1.Text+ '$'))
            {
                recieved.Text ="Message Recieved";
            }else
            {
                recieved.Text = "Error";

            }
        }
    }
}