String.prototype.format = function (args) { 
    var str = this; 
    return str.replace(String.prototype.format.regex, function(item) { 
        var intVal = parseInt(item.substring(1, item.length - 1)); 
        var replace; 
        if (intVal >= 0) { 
            replace = args[intVal]; 
        } else if (intVal === -1) { 
            replace = "{"; 
        } else if (intVal === -2) { 
            replace = "}"; 
        } else { 
            replace = ""; 
        } 
        return replace; 
    }); 
}; 
String.prototype.format.regex = new RegExp("{-?[0-9]+}", "g"); 

let list = document.getElementById("target_table");
httpGet("/list", function(text) {
    let recipes = text.split("\n");
    recipes.pop(); // remove blank string
    recipes.forEach(recipe => {
        if(recipe.length > 0) {
            let r = recipe.split(" ");
            for(let i = 0; i < r.length; i++) {
                r[i] = postDecode(r[i]);
            }

            list.innerHTML += `
                <tr id=\"{0}\" onclick="document.location.href = '/view.htm?id={0}'">
                    <td data-title="Nombre">{1}</td>
                    <td data-title="Comensales">{2}</td>
                    <td data-title="Ingredientes">{3}</td>
                </tr>
                `.format([r[0], r[1], r[2], "tomate"]);
        }
    });
});

function filter(input_id) {
    var input, filter, table, tr, td, i;
    
    table = document.getElementById("target_table");
    tr = table.getElementsByTagName("tr");
    for (i = 0; i < tr.length; i++) {
        for(let id = 0; id < 3; id++) {
            input = document.getElementById(input_id+id);
            filter = input.value.toUpperCase();
            
            td = tr[i].getElementsByTagName("td")[id];
            if(td) {
                txtValue = td.textContent || td.innerText;
                if (txtValue.toUpperCase().indexOf(filter) <= -1) {
                    tr[i].style.display = "none";
                    break;
                } else {
                    tr[i].style.display = "";
                }
            }
        }
    }
}

function filter_change() {
    let select = document.getElementById("filter_select").value;
    for(let i = 0; i < 4; i++) {
        let input = document.getElementById("in_filter"+i);
        if(i == select) {
            input.style.display = "inline-block";
        } else {
            input.style.display = "none";
        }
    }
}