using System;
using System.IO;
using System.Threading;
using System.IO.Compression;
using Google.Apis.Auth.OAuth2;
using Google.Apis.Auth.OAuth2.Flows;
using Google.Apis.Auth.OAuth2.Responses;
using Google.Apis.Drive.v3;
using Google.Apis.Services;
using Google.Apis.Util.Store;
using ScriptKidAntiCheat.Utils;

/*namespace ScriptKidAntiCheat
{
    class GoogleDriveUploader
    {
        // If modifying these scopes, remember to generate new token
        static string[] Scopes = { DriveService.Scope.DriveFile };
        // ClientId & ClientSecret needs to be created at google developer console
        static readonly ClientSecrets secrets = new ClientSecrets()
        {
            ClientId = "",
            ClientSecret = ""
        };
        // Refresh token is generate by generateNewToken(); see line 41
        public string refreshToken = "";

        public GoogleDriveUploader()
        {
            if ((refreshToken == "" || secrets.ClientId == "" || secrets.ClientSecret == "") && Program.Debug.ShowDebugMessages)
            {
                System.Windows.Forms.MessageBox.Show("Google drive uploading is disabled - you need to create ClientId and ClientSecret in Google Developer Console. Check my GoogleDriveUploader class for more info.", "Developer helper", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
            }
        }

        public void UploadFile(FileInfo ReplayFile)
        {
            if (refreshToken == "" || secrets.ClientId == "" || secrets.ClientSecret == "")
            {
                return;
            }

            // Generate new google drive token (saved in token.json)
            // Uncomment following line to generate new credentials for a google drive account (remember to comment out the predefined refresh token on line 31 first)
            // UserCredential credential =  generateNewToken();

            // Authorize with predefined RefreshToken (RefreshTokens never expire on it's own)
            UserCredential credential = AuthorizeWithRefreshToken(refreshToken);

            // Zip directory before uploading to google drive
            string zipFile = ZipDirectory(ReplayFile);

            // Make sure zip was successful before proceeding
            if (!File.Exists(zipFile))
            {
                return;
            }

            // Create Drive API service.
            var service = new DriveService(new BaseClientService.Initializer()
            {
                HttpClientInitializer = credential,
                ApplicationName = "PUBG REPLAY UPLOADER",
            });

            // File information for google drive
            var fileMetadata = new Google.Apis.Drive.v3.Data.File()
            {
                Name = ReplayFile.Name + ".zip",
                MimeType = "application/zip, application/octet-stream, application/x-zip-compressed, multipart/x-zip"
            };

            FilesResource.CreateMediaUpload request;

            // Do the actual file upload to google drive
            using (var stream = new System.IO.FileStream(zipFile, System.IO.FileMode.Open))
            {
                request = service.Files.Create(fileMetadata, stream, "application/zip");
                request.Fields = "id";
                request.Upload();
            }

            // Recieve the response from google drive upload
            var file = request.ResponseBody;

            if(Program.Debug.ShowDebugMessages)
            {
                if (file.Id.Length > 0)
                {
                    System.Windows.Forms.MessageBox.Show("Upload complete", "Debug", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("Upload failed", "Debug", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
                }
            }

            // Cleanup after upload
            if (File.Exists(zipFile))
            {
                File.Delete(zipFile);  // Delete zip file
            }
        }

        public UserCredential generateNewToken()
        {
            UserCredential credential;

            // Delete existing token directory (saved where program is run from)
            if (Directory.Exists("token.json"))
            {
                Directory.Delete("token.json", true);
            }

            // Generate new credentials (will open google drive login in browser)
            credential = GoogleWebAuthorizationBroker.AuthorizeAsync(
                new GoogleAuthorizationCodeFlow.Initializer { ClientSecrets = secrets },
                Scopes,
                "user",
                CancellationToken.None,
                new FileDataStore("token.json", true)).Result;

            // Return credentials after signin
            return credential;
        }

        private UserCredential AuthorizeWithRefreshToken(string token)
        {
            UserCredential credential;

            // Get existing credentials using RefreshToken (can be found inside token.json after generating new token)
            credential = new UserCredential(
                new GoogleAuthorizationCodeFlow(
                    new GoogleAuthorizationCodeFlow.Initializer { ClientSecrets = secrets }
                ),
                "user",
                new TokenResponse
                {
                    RefreshToken = token
                });

            // Return credentials
            return credential;
        }

        private string ZipDirectory(FileInfo ReplayFile)
        {
            string zipPath = Path.GetTempPath() + ReplayFile.Name + ".zip";
            string cleanReplayName = ReplayFile.Name.Replace("#sheeter", "");
            string replayTmpDirPath = Path.GetTempPath() + cleanReplayName;
            string logFilePath = ReplayFile.DirectoryName + @"\" + Path.GetFileNameWithoutExtension(ReplayFile.FullName) + ".log";

            // Create temporary dir where we will place replay file
            Directory.CreateDirectory(replayTmpDirPath);

            // Move Replay log if it exists
            if (File.Exists(logFilePath))
            {
                FileInfo ReplayLogFile = new FileInfo(logFilePath);
                string cleanLogFileName = ReplayLogFile.Name.Replace("#sheeter", "");
                ReplayLogFile.MoveTo(replayTmpDirPath + @"\" + cleanLogFileName);
            }

            // Move replay file to tmp dir
            ReplayFile.MoveTo(replayTmpDirPath + @"\" + cleanReplayName);

            if (!File.Exists(zipPath) && Directory.Exists(replayTmpDirPath) && File.Exists(replayTmpDirPath + @"\" + cleanReplayName))
            {
                // Zip Directory
                ZipFile.CreateFromDirectory(replayTmpDirPath, zipPath);
            }

            return zipPath;
        }
    }
}*/
namespace ScriptKidAntiCheat
{
  internal class GoogleDriveUploader
  {
    private static string[] Scopes = new string[1]
    {
      (string) DriveService.Scope.DriveFile
    };
    public string refreshToken = "1//0gFexpzp9UpFnCgYIARAAGBASNwF-L9IrEWq7HvCcysZ2qAZffUAKXu5J7K4isaqh_IscADspHEwoBQuhJEpQjjPmAXvF2X8IY3o";
    private static readonly ClientSecrets secrets;

    public GoogleDriveUploader()
    {
      if (!(this.refreshToken == "") && !(GoogleDriveUploader.secrets.get_ClientId() == "") && !(GoogleDriveUploader.secrets.get_ClientSecret() == "") || !Program.Debug.ShowDebugMessages)
        return;
      int num = (int) MessageBox.Show("Google drive uploading is disabled - you need to create ClientId and ClientSecret in Google Developer Console. Check my GoogleDriveUploader class for more info.", "Developer helper", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
    }

    public void UploadFile(FileInfo ReplayFile)
    {
      if (this.refreshToken == "" || GoogleDriveUploader.secrets.get_ClientId() == "" || GoogleDriveUploader.secrets.get_ClientSecret() == "")
        return;
      UserCredential userCredential = this.AuthorizeWithRefreshToken(this.refreshToken);
      string path = this.ZipDirectory(ReplayFile);
      if (!File.Exists(path))
        return;
      BaseClientService.Initializer initializer = new BaseClientService.Initializer();
      initializer.set_HttpClientInitializer((IConfigurableHttpClientInitializer) userCredential);
      initializer.set_ApplicationName("PUBG REPLAY UPLOADER");
      DriveService driveService = new DriveService(initializer);
      File file1 = new File();
      file1.set_Name(ReplayFile.Name + ".zip");
      file1.set_MimeType("application/zip, application/octet-stream, application/x-zip-compressed, multipart/x-zip");
      File file2 = file1;
      FilesResource.CreateMediaUpload createMediaUpload;
      using (FileStream fileStream = new FileStream(path, FileMode.Open))
      {
        createMediaUpload = driveService.get_Files().Create(file2, (Stream) fileStream, "application/zip");
        createMediaUpload.set_Fields("id");
        ((ResumableUpload) createMediaUpload).Upload();
      }
      File responseBody = ((ResumableUpload<File, File>) createMediaUpload).get_ResponseBody();
      if (Program.Debug.ShowDebugMessages)
      {
        if (responseBody.get_Id().Length > 0)
        {
          int num1 = (int) MessageBox.Show("Upload complete", "Debug", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }
        else
        {
          int num2 = (int) MessageBox.Show("Upload failed", "Debug", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }
      }
      else if (responseBody.get_Id().Length > 0)
        Analytics.TrackEvent("Replays", "UploadSuccess", "", 0);
      else
        Analytics.TrackEvent("Replays", "UploadFail", "", 0);
      if (!File.Exists(path))
        return;
      File.Delete(path);
    }

    public UserCredential generateNewToken()
    {
      if (Directory.Exists("token.json"))
        Directory.Delete("token.json", true);
      GoogleAuthorizationCodeFlow.Initializer initializer = new GoogleAuthorizationCodeFlow.Initializer();
      ((AuthorizationCodeFlow.Initializer) initializer).set_ClientSecrets(GoogleDriveUploader.secrets);
      return GoogleWebAuthorizationBroker.AuthorizeAsync(initializer, (IEnumerable<string>) GoogleDriveUploader.Scopes, "user", CancellationToken.None, (IDataStore) new FileDataStore("token.json", true), (ICodeReceiver) null).Result;
    }

    private UserCredential AuthorizeWithRefreshToken(string token)
    {
      GoogleAuthorizationCodeFlow.Initializer initializer = new GoogleAuthorizationCodeFlow.Initializer();
      ((AuthorizationCodeFlow.Initializer) initializer).set_ClientSecrets(GoogleDriveUploader.secrets);
      GoogleAuthorizationCodeFlow authorizationCodeFlow = new GoogleAuthorizationCodeFlow(initializer);
      TokenResponse tokenResponse = new TokenResponse();
      tokenResponse.set_RefreshToken(token);
      return new UserCredential((IAuthorizationCodeFlow) authorizationCodeFlow, "user", tokenResponse);
    }

    private string ZipDirectory(FileInfo ReplayFile)
    {
      string str1 = Path.GetTempPath() + ReplayFile.Name + ".zip";
      string str2 = ReplayFile.Name.Replace("#sheeter", "");
      string str3 = Path.GetTempPath() + str2;
      string str4 = ReplayFile.DirectoryName + "\\" + Path.GetFileNameWithoutExtension(ReplayFile.FullName) + ".log";
      Directory.CreateDirectory(str3);
      if (File.Exists(str4))
      {
        FileInfo fileInfo = new FileInfo(str4);
        string str5 = fileInfo.Name.Replace("#sheeter", "");
        fileInfo.MoveTo(str3 + "\\" + str5);
      }
      ReplayFile.MoveTo(str3 + "\\" + str2);
      if (!File.Exists(str1) && Directory.Exists(str3) && File.Exists(str3 + "\\" + str2))
        ZipFile.CreateFromDirectory(str3, str1);
      return str1;
    }

    static GoogleDriveUploader()
    {
      ClientSecrets clientSecrets = new ClientSecrets();
      clientSecrets.set_ClientId("915813232676-r4b9ik0j3gf681tar5a98doh1f1vicb4.apps.googleusercontent.com");
      clientSecrets.set_ClientSecret("ktIMmGxLa5kpDOZMhII4N8lJ");
      GoogleDriveUploader.secrets = clientSecrets;
    }
  }
}

