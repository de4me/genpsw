

# genpsw

Console application for generating custom passwords.


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

genpsw [FLAGS] [PASSWORD LENGTH] [NUMBER]

WHERE:


#### FLAGS

 n 	- Use decimal numbers \*

 l 	- Use lowercase letters \*

 L 	- Use uppercase letters \*

 s 	- Use symbols \*

 S 	- Use extended symbols

 h 	- Use hex numbers

 r 	- Remove similar letters

 1 	- Do not repeat letters

\*: Set by Default.


#### PASSWORD LENGTH

Length of the generated password.

Default: 32.


#### NUMBER

Number of passwords to generate so that you can choose one of/or generate several passwords at once.

Default: 1.


```bash
genpsw nlLsr 64 5
```


<p align="center">
<img width="554" src="https://github.com/de4me/genpsw/blob/main/genpsw.png">
</p>
