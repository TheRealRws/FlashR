using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace Arduino_App_16
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();
        }
        //once the button "Sent message" is pressed, the function will call a new communications Page"
        async void NavigateTo_Communication(object sender, EventArgs e)
        {
                await Navigation.PushAsync(new CommunicationPage());
            
        }
        //once the button "Settings" is pressed, the function will call a new Settings Page"
        async void NavigateTo_Settings(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new SettingsPage());

        }
    }
}
