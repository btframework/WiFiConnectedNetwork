using System;
using System.Windows.Forms;

using wclCommon;
using wclWiFi;

namespace ConnectedNetwork
{
    public partial class fmMain : Form
    {
        private wclWiFiClient Client;

        public fmMain()
        {
            InitializeComponent();
        }

        private void Trace(String Str)
        {
            lbInfo.Items.Add(Str);
        }
        
        private void ShowError(String Str, Int32 Error)
        {
            String Framework;
            String Category;
            String Constant;
            String Description;
            if (wclHelpers.GetErrorInfo(Error, out Framework, out Category, out Constant, out Description))
                Trace(Str + ": 0x" + Error.ToString("X8") + " [" + Constant + "]: " + Description);
            else
                Trace(Str + ": 0x" + Error.ToString("X8"));
        }

        private void btGetInfo_Click(object sender, EventArgs e)
        {
            /*
        Bss: TwclWiFiBss;
            begin*/
            Client = new wclWiFiClient();
            Int32 Res = Client.Open();
            if (Res != wclErrors.WCL_E_SUCCESS)
                ShowError("Open failed", Res);
            else
            {
                wclWiFiInterfaceData[] Ifaces;
                Res = Client.EnumInterfaces(out Ifaces);
                if (Res != wclErrors.WCL_E_SUCCESS)
                    ShowError("Enum interfaces failed", Res);
                else
                {
                    if (Ifaces == null || Ifaces.Length == 0)
                        Trace("No WiFi interfaces found");
                    else
                    {
                        foreach (wclWiFiInterfaceData Data in Ifaces)
                        {
                            Guid Id = Data.Id;
                            String Descr = Data.Description;

                            wclWiFiInterface Iface = new wclWiFiInterface(Id);
                            Res = Iface.Open();
                            if (Res != wclErrors.WCL_E_SUCCESS)
                                ShowError("Open interface " + Descr + " failed", Res);
                            else
                            {
                                Trace("Opened " + Descr);
                                wclWiFiInterfaceState State;
                                Res = Iface.GetState(out State);
                                if (Res != wclErrors.WCL_E_SUCCESS)
                                    ShowError("  Get state failed", Res);
                                else
                                {
                                    String Str = "  Interface state: ";
                                    switch (State)
                                    {
                                        case wclWiFiInterfaceState.isNotReady:
                                            Str = Str + "Not ready";
                                            break;
                                        case wclWiFiInterfaceState.isConnected:
                                            Str = Str + "Connected";
                                            break;
                                        case wclWiFiInterfaceState.isAdHocFormed:
                                            Str = Str + "AdHoc formed";
                                            break;
                                        case wclWiFiInterfaceState.isDisconnecting:
                                            Str = Str + "Disconnecting";
                                            break;
                                        case wclWiFiInterfaceState.isDisconnected:
                                            Str = Str + "Disconnected";
                                            break;
                                        case wclWiFiInterfaceState.isAssociating:
                                            Str = Str + "Associating";
                                            break;
                                        case wclWiFiInterfaceState.isDiscovering:
                                            Str = Str + "Discovering";
                                            break;
                                        case wclWiFiInterfaceState.isAuthenticating:
                                            Str = Str + "Authenticating";
                                            break;
                                        default:
                                            Str = Str + "Unknown";
                                            break;
                                    }
                                    Trace(Str);

                                    if (State == wclWiFiInterfaceState.isConnected)
                                    {
                                        wclWiFiConnectionAttributes Attr;
                                        Res = Iface.GetConnectionAttributes(out Attr);
                                        if (Res != wclErrors.WCL_E_SUCCESS)
                                            ShowError("  Get connection attributes failed", Res);
                                        else
                                        {
                                            Str = "  Connecting mode: ";
                                            switch (Attr.Mode)
                                            {
                                                case wclWiFiConnectionMode.cmProfile:
                                                    Str = Str + "Profile";
                                                    break;
                                                case wclWiFiConnectionMode.cmTemporaryProfile:
                                                    Str = Str + "Temporary Profile";
                                                    break;
                                                case wclWiFiConnectionMode.cmDiscoverySecure:
                                                    Str = Str + "Discovery Secure";
                                                    break;
                                                case wclWiFiConnectionMode.cmDiscoveryUnsecure:
                                                    Str = Str + "Discovery Unsecure";
                                                    break;
                                                case wclWiFiConnectionMode.cmAuto:
                                                    Str = Str + "Auto";
                                                    break;
                                                case wclWiFiConnectionMode.cmInvalid:
                                                    Str = Str + "Invalid";
                                                    break;
                                                default:
                                                    Str = Str + "Unknown";
                                                    break;
                                            }
                                            Trace(Str);

                                            if (Attr.Mode == wclWiFiConnectionMode.cmProfile || Attr.Mode == wclWiFiConnectionMode.cmTemporaryProfile)
                                                Trace("  Profile: " + Attr.ProfileName);

                                            Trace("  SSID: " + Attr.Association.Ssid);
                                            Trace("  BSS ID: " + Attr.Association.BssId);
                                            Trace("  SQ: " + Attr.Association.SignalQuality.ToString());
                                            Trace("  RX Rate: " + Attr.Association.RxRate.ToString());
                                            Trace("  TX Rate: " + Attr.Association.TxRate.ToString());

                                            UInt32 Ch;
                                            Res = Iface.GetChannel(out Ch);
                                            if (Res != wclErrors.WCL_E_SUCCESS)
                                                ShowError("  Get channel failed", Res);
                                            else
                                                Trace("  Channel: " + Ch.ToString());

                                            wclWiFiBss[] BssList;
                                            Res = Client.EnumBss(Id, Attr.Association.Ssid, Attr.Association.BssType,
                                                Attr.Security.SecurityEnabled, out BssList);
                                            if (Res != wclErrors.WCL_E_SUCCESS)
                                                ShowError("  Enum BSS failed", Res);
                                            else
                                            {
                                                if (BssList == null || BssList.Length == 0)
                                                    Trace("  No BSS found");
                                                else
                                                {
                                                    foreach (wclWiFiBss Bss in BssList)
                                                    {
                                                        if (Bss.Mac == Attr.Association.BssId)
                                                        {
                                                            Trace("  BSS RSSI: " + Bss.Rssi.ToString());
                                                            Trace("  BSS LQ: " + Bss.LinkQuality.ToString());
                                                            Trace("  BSS Center FREQ: " + Bss.ChCenterFrequency.ToString());
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                Iface.Close();
                            }
                        }
                    }
                }
                Client.Close();
            }
        }
    }
}
