pipeline {
    agent none

	parameters {
        // With the upgrade to Fedora 31 and podman the Jenkins docker agent is no longer supported

        // Build on Fedora's default compiler as first parallel stages
        booleanParam name: 'Use_gcc9', defaultValue: true, description: 'Build/test using gcc9'
        booleanParam name: 'Use_clang9', defaultValue: true, description: 'Build/test using clang9'
        // Sanitizer tests in parallel stages
        booleanParam name: 'Run_sanitizers', defaultValue: false, description: 'Build/test using sanitizers'
        // Build alternative compilers in parallel stages
        booleanParam name: 'Use_gcc4', defaultValue: false, description: 'Build/test using gcc4'
        booleanParam name: 'Use_gcc5', defaultValue: false, description: 'Build/test using gcc5'
        booleanParam name: 'Use_gcc6', defaultValue: false, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: false, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc8', defaultValue: false, description: 'Build/test using gcc8'
	}
    stages {
        stage('Parallel system compiler stages') {
            failFast true
            parallel {
                stage('gcc9.3.1') {
                    when {
                        environment name: 'Use_gcc9', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        // Enable clang-tidy checks on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 8.4.0"
                        sh './container.sh --image=serf-cpp-gcc931 --dir=gcc931 --cmake="-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DENABLE_CPPCHECK=TRUE"'
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc931/unittests.xml'
                        }
                    }
                }
                stage('clang9') {
                    when {
                        environment name: 'Use_clang9', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        // Enable cppcheck on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using clang 9"
                        sh './container.sh --image=serf-cpp-gcc931 --dir=clang9 --cc=clang --cxx=clang++'
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'clang9/unittests.xml'
                        }
                    }
                }
            }
        }

        stage('Parallel sanitizer stages') {
            failFast true
            parallel {
                stage('asan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        sh './container.sh --podman="--cap-add SYS_PTRACE" --image=serf-cpp-gcc931 --dir=asan --cmake="-DCMAKE_BUILD_TYPE=asan"'
                    }
                }

                stage('tsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        sh './container.sh --image=serf-cpp-gcc931 --dir=tsan --cmake="-DCMAKE_BUILD_TYPE=tsan"'
                    }
                }

                stage('ubsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        sh './container.sh --image=serf-cpp-gcc931 --dir=ubsan --cmake="-DCMAKE_BUILD_TYPE=ubsan"'
                    }
                }
            }
        }

        stage('Parallel alternate compiler stages') {
            failFast true
            parallel {
                stage('gcc8.4.0') {
                    when {
                        environment name: 'Use_gcc8', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 8.4.0"
                        sh './container.sh --image=serf-cpp-gcc840 --dir=gcc840 -cc=/opt/gcc840/bin/gcc -cxx=/opt/gcc840/bin/g++'                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc840/unittests.xml'
                        }
                    }
                }

                stage('gcc7.4.0') {
                    when {
                        environment name: 'Use_gcc7', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 7.4.0"
                        sh './container.sh --image=serf-cpp-gcc840 --dir=gcc740 -cc=/opt/gcc740/bin/gcc -cxx=/opt/gcc740/bin/g++'
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc740/unittests.xml'
                        }
                    }
                }

                stage('gcc6.5.0') {
                    when {
                        environment name: 'Use_gcc6', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 6.5.0"
                        sh './container.sh --image=serf-cpp-gcc650 --dir=gcc650 -cc=/opt/gcc650/bin/gcc -cxx=/opt/gcc650/bin/g++'
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc650/unittests.xml'
                        }
                    }
                }

                stage('gcc5.3.0') {
                    when {
                        environment name: 'Use_gcc5', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 5.3.0"
                        sh './container.sh --image=serf-cpp-gcc530 --dir=gcc530 -cc=/usr/bin/gcc53 -cxx=/usr/bin/g++53'                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc530/unittests.xml'
                        }
                    }
                }

                stage('gcc4.9.3') {
                    when {
                        environment name: 'Use_gcc4', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 4.9.3"
                        sh './container.sh --image=serf-cpp-gcc493 --dir=gcc493 -cc=/usr/bin/gcc49 -cxx=/usr/bin/g++49'
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc493/unittests.xml'
                        }
                    }
                }
            }
        }
    }
    
}
