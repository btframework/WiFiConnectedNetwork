unit main;

interface

uses
  Forms, Controls, StdCtrls, Classes, wclWiFi;

type
  TfmMain = class(TForm)
    wclWiFiClient: TwclWiFiClient;
    Button1: TButton;
    lbInfo: TListBox;
    procedure Button1Click(Sender: TObject);

  private
    procedure Trace(const Str: string);
    procedure ShowError(const Str: string; const Error: Integer);
  end;

var
  fmMain: TfmMain;

implementation

uses
  wclErrors, wclHelpers, SysUtils;

{$R *.dfm}

procedure TfmMain.Button1Click(Sender: TObject);
var
  Res: Integer;
  Ifaces: TwclWiFiInterfaces;
  i: Integer;
  Iface: TwclWiFiInterface;
  Id: TGUID;
  Descr: string;
  Attr: TwclWiFiConnectionAttributes;
  State: TwclWiFiInterfaceState;
  Str: string;
  Ch: Cardinal;
  BssList: TwclWiFiBssArray;
  j: Integer;
  Bss: TwclWiFiBss;
begin
  Res := wclWiFiClient.Open;
  if Res <> WCL_E_SUCCESS then
    ShowError('Open failed', Res)

  else begin
    Res := wclWiFiClient.EnumInterfaces(Ifaces);
    if Res <> WCL_E_SUCCESS then
      ShowError('Enum interfaces failed', Res)

    else begin
      if Length(Ifaces) = 0 then
        Trace('No WiFi interfaces found')

      else begin
        for i := 0 to Length(Ifaces) - 1 do begin
          Id := Ifaces[i].Id;
          Descr := Ifaces[i].Description;

          Iface := TwclWiFiInterface.Create(Id);
          Res := Iface.Open;
          if Res <> WCL_E_SUCCESS then
            ShowError('Open interface ' + Descr + ' failed', Res)

          else begin
            Trace('Opened ' + Descr);
            Res := Iface.GetState(State);
            if Res <> WCL_E_SUCCESS then
              ShowError('  Get state failed', Res)

            else begin
              Str := '  Interface state: ';
              case State of
                isNotReady:
                  Str := Str + 'Not ready';
                isConnected:
                  Str := Str + 'Connected';
                isAdHocFormed:
                  Str := Str + 'AdHoc formed';
                isDisconnecting:
                  Str := Str + 'Disconnecting';
                isDisconnected:
                  Str := Str + 'Disconnected';
                isAssociating:
                  Str := Str + 'Associating';
                isDiscovering:
                  Str := Str + 'Discovering';
                isAuthenticating:
                  Str := Str + 'Authenticating';
                else
                  Str := Str + 'Unknown';
              end;
              Trace(Str);

              if State = isConnected then begin
                Res := Iface.GetConnectionAttributes(Attr);
                if Res <> WCL_E_SUCCESS then
                  ShowError('  Get connection attributes failed', Res)

                else begin
                  Str := '  Connecting mode: ';
                  case Attr.Mode of
                    cmProfile:
                      Str := Str + 'Profile';
                    cmTemporaryProfile:
                      Str := Str + 'Temporary Profile';
                    cmDiscoverySecure:
                      Str := Str + 'Discovery Secure';
                    cmDiscoveryUnsecure:
                      Str := Str + 'Discovery Unsecure';
                    cmAuto:
                      Str := Str + 'Auto';
                    cmInvalid:
                      Str := Str + 'Invalid';
                    else
                      Str := Str + 'Unknown';
                  end;
                  Trace(Str);

                  if (Attr.Mode = cmProfile) or (Attr.Mode = cmTemporaryProfile)
                  then
                    Trace('  Profile: ' + Attr.ProfileName);

                  Trace('  SSID: ' + Attr.Association.Ssid);
                  Trace('  BSS ID: ' + Attr.Association.BssId);
                  Trace('  SQ: ' + IntToStr(Attr.Association.SignalQuality));
                  Trace('  RX Rate: ' + IntToStr(Attr.Association.RxRate));
                  Trace('  TX Rate: ' + IntToStr(Attr.Association.TxRate));

                  Res := Iface.GetChannel(Ch);
                  if Res <> WCL_E_SUCCESS then
                    ShowError('  Get channel failed', Res)
                  else
                    Trace('  Channel: ' + IntToStr(Ch));

                  Res := wclWiFiClient.EnumBss(Id, Attr.Association.Ssid,
                    Attr.Association.BssType, Attr.Security.SecurityEnabled,
                    BssList);
                  if Res <> WCL_E_SUCCESS then
                    ShowError('  Enum BSS failed', Res)

                  else begin
                    if Length(BssList) = 0 then
                      Trace('  No BSS found')

                    else begin
                      for j := 0 to Length(BssList) - 1 do begin
                        Bss := BssList[i];
                        if Bss.Mac = Attr.Association.BssId then begin
                          Trace('  BSS RSSI: ' + IntToStr(Bss.Rssi));
                          Trace('  BSS LQ: ' + IntToStr(Bss.LinkQuality));
                          Trace('  BSS Center FREQ: ' + IntToStr(Bss.ChCenterFrequency));
                        end;
                      end;
                    end;
                  end;
                end;
              end;
            end;
            Iface.Close;
          end;
          Iface.Free;
        end;
      end;
    end;
    wclWiFiClient.Close;
  end;
end;

procedure TfmMain.ShowError(const Str: string; const Error: Integer);
var
  Framework: string;
  Category: string;
  Constant: string;
  Description: string;
begin
  if wclGetErrorInfo(Error, Framework, Category, Constant, Description) then
    Trace(Str + ': 0x' + IntToHex(Error, 8) + ' [' + Constant + ']: ' + Description)
  else
    Trace(Str + ': 0x' + IntToHex(Error, 8));
end;

procedure TfmMain.Trace(const Str: string);
begin
  lbInfo.Items.Add(Str);
end;

end.
