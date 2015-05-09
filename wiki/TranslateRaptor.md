## Translate Raptor ##

It's easy to translate raptor using the Qt Linguist tools

  1. Use the lupdate tool to generate/update the translation file `*`.ts
```
lupdate src/ -ts src/translations/raptor_{new_language}.ts
```
  1. Launch the linguist tool and translate the `*`.ts file
```
linguist raptor_{new_language}.ts
```
  1. Launch raptor build to generate the .qm file

## Use the new translation file ##
Just copy the translation .qm file to the raptor translation path.
Launch raptor. In the settings window, you can now select the new language. You'll have to restart raptor to have it translated.