import builder from "yargs";
import fs from "fs"
import path from "path"
import { syntaxColoring } from "./syntaxColoring.js";
const yargs = builder(process.argv)
const argv = yargs
  .option('input', {
    description: 'Input folder path',
    alias: 'i',
    type: 'string'
  })
  .option('output', {
    description: 'Output file path',
    alias: 'o',
    type: 'string'
  })
  .help()
  .alias('help', 'h')
  .argv;

  if(!argv.input || !argv.output){
    yargs.showHelp()

  }
  else{
    const files = getAllFiles(argv.input,/\.psc$/)

    let content = ""

    for(const file of files){
        const raw = fs.readFileSync(file, { encoding: 'utf8'})
        const colored = syntaxColoring(raw)
        const fileName = file.match(/([^\\]*)\.psc/)[1]
        content += 
        `[b][size=5][center]${fileName}[/center][/size][/b]`+
        `[spoiler]` +
        colored +
        `[/spoiler]\n`
    }
    
    fs.writeFileSync(argv.output, content, { encoding: 'utf8' });
  }

  function getAllFiles(folderPath, match) {
    let files = [];
    return loop(folderPath)
    function loop(folderPath){
        // Read the current directory
        const entries = fs.readdirSync(folderPath);
      
        // Iterate through each entry
        for (const entry of entries) {
          const entryPath = path.join(folderPath, entry);
      
          if (fs.statSync(entryPath).isDirectory()) {
            // If it's a directory, recursively get all files in the subdirectory
            const subFiles = getAllFiles(entryPath);
            if(match !== undefined){
                files = [...files, ...subFiles.filter(x=>x.match(match))];
            }
            else{
                files = [...files, ...subFiles];
            }
          } else {
            // If it's a file, add it to the list of files
            if(match == null || entryPath.match(match)){
                files.push(entryPath);
            }
          }
        }
      
        return files;
    }

  }

