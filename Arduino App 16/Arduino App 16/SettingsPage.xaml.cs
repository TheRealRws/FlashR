using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace Arduino_App_16
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SettingsPage : ContentPage
    {
        public SettingsPage()
        {
            InitializeComponent();
        }
        //test the connections with the Arduino via the Wifi
        private void Test_Connection_Clicked(object sender, EventArgs e)
        {
            string ip = EntryIP.Text;
            int port;
            if (Int32.TryParse(EntryPort.Text, out port))
            {
                string reply = Connection.TestConnection(ip, port);
                LabelConnected.Text = reply;
                if (reply == "Connection established, ECHO received")
                {
                    LabelConnected.TextColor = Color.Green;

                }
                else
                {
                    LabelConnected.TextColor = Color.Red;
                }
            }
            else
            {
                LabelConnected.Text = "Port not a number";
                LabelConnected.TextColor = Color.Red;
            }
        }
    }
}