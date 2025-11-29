

# genpsw

The genpsw command line tool generates random passwords.
It can be used directly from the terminal with various options to customize the output.
Below are detailed examples showing how to use genpsw to generate passwords of different lengths, with or without symbols, and more.

Also you can use genpsw to generate random Mac Addresses (unicast locally administered), UUIDs (version 4), CLSIDs (version 4).


```bash
genpsw nlLs 64 5
```

```bash
genpsw strong 5
```

```bash
genpsw l "1??-2??-3??-4??" 5
```


## Requirements

* MS Visual Studio 2019 for Windows;
* Xcode 16.4 for macOS;


## How To Build

<details>
<summary>Xcode</summary>

    Open genpsw.xcodeproj and build.

</details>

<details>
<summary>MS Visual Studio</summary>

    Open genpsw.sln and build.

</details>


## Command Arguments

genpsw [-h] [-v] [FLAGS] [[LENGTH] | [MASK]] [COUNT]


#### WHERE

| Args | Description |
| :- | :- |
| -v | Show version |
| -h, -? | Show help |

| Flags | Description |
| :- | :- |
| n | Use decimal numbers \*
| l | Use lowercase letters \*
| L | Use uppercase letters \*
| s | Use symbols \*
| S | Use extended symbols
| h | Use hex numbers
| r | Remove similar letters
| 1 | Do not repeat letters
|   | \* - Set by Default


#### MASK

Mask for generated passwords.
In mask every occurence of '?' will be replaced with a generated symbol.

Or you can use predefined masks.

| Mask | Description |
| :----- | :- |
| strong | Strong password |
| good | Good password |
| weak | Weak password |
| wifi | Wi-Fi password |
| mac | A unique identifier assigned to a network interface controller |
| uuid | 128-bit number designed to be a unique identifier for objects in computer systems |
| clsid | Globally Unique Identifier that identifies a COM class object |


#### LENGTH

Length of the generated password.

*Default: 32.*


#### COUNT

Number of passwords to generate so that you can choose one of/or generate several passwords at once.

*Default: 1.*


<p align="center">
<img width="554" src="https://github.com/de4me/genpsw/blob/main/genpsw.png">
</p>
