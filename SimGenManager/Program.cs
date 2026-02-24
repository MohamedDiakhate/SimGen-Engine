using System;
using System.IO;
using System.Text.Json;
using System.Diagnostics;

Console.WriteLine("=== SimGen++ C# Pipeline Manager ===");


var config = new SimConfig();

//Ask for inputs (with safe fallbacks)
Console.Write("Enter image width [default 1920]: ");
if (int.TryParse(Console.ReadLine(), out int w)) config.Width = w;

Console.Write("Enter image height [default 1080]: ");
if (int.TryParse(Console.ReadLine(), out int h)) config.Height = h;

Console.Write("Enter sample count [default 100]: ");
if (int.TryParse(Console.ReadLine(), out int s)) config.Samples = s;

//Generate the JSON file
string jsonString = JsonSerializer.Serialize(config, new JsonSerializerOptions { WriteIndented = true });
File.WriteAllText("config.json", jsonString);

Console.WriteLine("\n[SUCCESS] Generated config.json:");
Console.WriteLine(jsonString);


Console.WriteLine("\n[SYSTEM] Booting C++ Engine in Docker...");

var processInfo = new ProcessStartInfo
{
    FileName = "/bin/zsh",
    Arguments = "-c \"docker run --rm simgen-engine\"", 
    RedirectStandardOutput = true,
    RedirectStandardError = true, 
    UseShellExecute = false,
    CreateNoWindow = true
};

//Intercept the C++ output and bring it into our C# app
using var process = Process.Start(processInfo);
if (process != null)
{
    // Read both standard output and error output
    string output = process.StandardOutput.ReadToEnd();
    string error = process.StandardError.ReadToEnd();
    process.WaitForExit();
    
    Console.WriteLine("\n--- C++ ENGINE TERMINAL OUTPUT ---");
    Console.WriteLine(output);
    

    if (!string.IsNullOrWhiteSpace(error))
    {
        Console.WriteLine("--- C++ ENGINE ERRORS / WARNINGS ---");
        Console.WriteLine(error);
    }
    Console.WriteLine("----------------------------------");
}

Console.WriteLine("\n=== Pipeline Execution Finished ===");



public class SimConfig
{
    public int Width { get; set; } = 1920;
    public int Height { get; set; } = 1080;
    public int Samples { get; set; } = 100;
    public string OutputFile { get; set; } = "render_output.png";
}