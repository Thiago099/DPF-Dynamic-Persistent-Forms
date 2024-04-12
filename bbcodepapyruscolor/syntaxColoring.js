export { syntaxColoring }

const patterns = [
    //priority
    {
        pattern:/;.*/gi,
        key:"comment",
    },
    {
        pattern:/\d+/gi,
        color:"#B5CF9A"
    },

    //symbols
    {
        pattern:/\".*?\"/gi,
        color:"#CE723C",
    },
    {
        pattern:/\(|\)/gi,
        color:"#FEB519",
    },
    {
        pattern:/[^\s\.]*?\(/gi,
        color:"#DDDDAA"
    },

    // specific

    {
        pattern:/(?<=\s|^|\])(?:event|endevent|if|endif|while|endwhile|state|endstate|function)(?=\s|$|\[)/gi,
        color: "#C486C1",
    },
    {
        pattern:/(?<=\s|\^|\])(?:property|auto|global|native|extends|hidden|false|true)(?=\s|$|\[)/gi,
        color:"#3E9CCA"
    },
    {
        pattern:/(?<=\s|^|\])(?:scriptname)(?=\s|$|\[)/gi,
        color:"#33BAB1"
    }
]
function getUniqueId(num) {
    let result = '';
    while (num >= 0) {
        const charCode = num % 26 + 97; 
        result = String.fromCharCode(charCode) + result;
        num = Math.floor(num / 26) - 1;
    }
    return result;
}

function syntaxColoring(input){
    let index = 0
    const commentData = {}

    for(const {pattern, color,key} of patterns){
        if(key == "comment"){
            input = input.replace(pattern, x=>{
                const id = getUniqueId(index++)
                commentData[id] = x
                return `[comment]${id}[/comment]`
            })
        }
        else{
            input = input.replace(pattern, x=>{
                return `[color=${color}]${x}[/color]`
            })
        }
        
    }
    
    input = input.replace(/\[comment\](.*?)\[\/comment\]/g,(_,g1)=>{
        return `[color=#6A9955]${commentData[g1]}[/color]`
    })

    return input
}

